"""带缓存的多轮对话状态追踪器：线程安全状态树、LRU 缓存、层级失效和派生视图。
练习约定：路径为字符串元组，() 表示根；原始状态是 JSON 风格 dict，读取返回深拷贝。
更新 /profile 会清除其父/子路径缓存，并级联清除依赖这些路径的派生视图。
视图只依赖原始状态路径；计算函数纯函数，在锁外计算一致快照，版本变化时不回填旧缓存。
85% 命中率依赖流量；main 给出同一视图重复读取的样例，不能保证任意工作负载达到 85%。
读取成本为路径遍历/计算及数据拷贝；写入 O(会话状态大小+缓存容量×视图依赖数)。
缓存空间 O(capacity×记录大小)，会话状态另计；drop_session 回收会话。
运行：python3 conversation_state_cache.py
"""
from collections import OrderedDict
from copy import deepcopy
from threading import RLock


def _path(value):
    if not isinstance(value, (tuple, list)) or any(not isinstance(part, str) or not part for part in value):
        raise ValueError("path must be a tuple/list of nonempty strings")
    return tuple(value)


def _overlap(left, right):
    return left[:len(right)] == right or right[:len(left)] == left


class ConversationStateTracker:
    def __init__(self, capacity=128):
        if type(capacity) is not int or capacity <= 0:
            raise ValueError("capacity must be positive")
        self._capacity = capacity
        self._states = {}
        self._views = {}
        self._cache = OrderedDict()
        self._revision = 0
        self._hits = self._misses = 0
        self._lock = RLock()

    def _remember(self, key, value):
        self._cache[key] = deepcopy(value)
        self._cache.move_to_end(key)
        while len(self._cache) > self._capacity:
            self._cache.popitem(last=False)

    def _read(self, session, path):
        value = self._states[session]
        for segment in path:
            value = value[segment]
        return value

    def set_state(self, session, path, value):
        if not isinstance(session, str) or not session:
            raise ValueError("nonempty session id required")
        path = _path(path)
        with self._lock:
            # 在副本上修改，非法中间路径不会留下半次更新。
            root = deepcopy(self._states.get(session, {}))
            if not path:
                if not isinstance(value, dict):
                    raise ValueError("session root must be dict")
                root = deepcopy(value)
            else:
                parent = root
                for segment in path[:-1]:
                    if segment not in parent:
                        parent[segment] = {}
                    parent = parent[segment]
                    if not isinstance(parent, dict):
                        raise ValueError("intermediate state must be dict")
                parent[path[-1]] = deepcopy(value)
            self._states[session] = root
            self._revision += 1
            for key in list(self._cache):
                sid, kind, identity = key
                if sid != session:
                    continue
                dependencies = (identity,) if kind == "state" else self._views[identity][0]
                if any(_overlap(path, dependency) for dependency in dependencies):
                    del self._cache[key]

    def get_state(self, session, path=()):
        path = _path(path)
        key = (session, "state", path)
        with self._lock:
            if key in self._cache:
                self._hits += 1
                self._cache.move_to_end(key)
                return deepcopy(self._cache[key])
            self._misses += 1
            value = deepcopy(self._read(session, path))
            self._remember(key, value)
            return value

    def register_view(self, name, dependencies, compute):
        if not isinstance(name, str) or not name or not callable(compute):
            raise ValueError("invalid view")
        paths = tuple(_path(path) for path in dependencies)
        with self._lock:
            self._views[name] = (paths, compute)
            self._revision += 1
            for key in list(self._cache):
                if key[1:] == ("view", name):
                    del self._cache[key]

    def get_view(self, session, name):
        key = (session, "view", name)
        with self._lock:
            if key in self._cache:
                self._hits += 1
                self._cache.move_to_end(key)
                return deepcopy(self._cache[key])
            self._misses += 1
            definition = self._views[name]
            dependencies, compute = definition
            arguments = [deepcopy(self._read(session, path)) for path in dependencies]
            revision = self._revision
        value = compute(arguments)
        with self._lock:
            if self._revision == revision:
                self._remember(key, value)
        return deepcopy(value)

    def drop_session(self, session):
        with self._lock:
            self._states.pop(session, None)
            self._revision += 1
            for key in list(self._cache):
                if key[0] == session:
                    del self._cache[key]

    def statistics(self):
        with self._lock:
            reads = self._hits + self._misses
            return {"hits": self._hits, "misses": self._misses,
                    "hit_rate": self._hits / reads if reads else 0.0}


if __name__ == "__main__":
    tracker = ConversationStateTracker()
    tracker.set_state("s1", (), {"profile": {"name": "Alice"}, "turn": 1})
    tracker.register_view("greeting", [("profile", "name")], lambda values: "Hello " + values[0])
    for _ in range(100):
        tracker.get_view("s1", "greeting")
    print("repeated-read workload:", tracker.statistics())
    tracker.set_state("s1", ("profile",), {"name": "Bob"})
    print(tracker.get_view("s1", "greeting"))
