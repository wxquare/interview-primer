"""扁平 JSON 列表还原树。练习约定：id 为 int/str（不接受 bool），parent_id=None 为根。
允许多个根，保持输入顺序；拒绝重复 ID、缺失父节点、自环/多节点环和已有 children 字段。
不修改输入，保留其他字段。时间 O(n + 字段拷贝大小)，空间同阶。
运行：python3 json_flat_tree.py
"""
from copy import deepcopy
import json


def build_forest(records):
    nodes, parents = {}, {}
    for record in records:
        if not isinstance(record, dict) or "id" not in record or "parent_id" not in record:
            raise ValueError("each row needs id and parent_id")
        identity, parent = record["id"], record["parent_id"]
        if type(identity) not in (int, str) or (parent is not None and type(parent) not in (int, str)):
            raise ValueError("id/parent_id must be int or str")
        if identity in nodes or "children" in record:
            raise ValueError("duplicate id or reserved children field")
        nodes[identity] = dict(deepcopy(record), children=[])
        parents[identity] = parent
    for parent in parents.values():
        if parent is not None and parent not in nodes:
            raise ValueError("parent id does not exist")

    # 按父链着色，每个节点最多走一次，避免递归深链溢出。
    colors = {}
    for identity in nodes:
        current, trail = identity, []
        while current is not None and colors.get(current, 0) == 0:
            colors[current] = 1
            trail.append(current)
            current = parents[current]
        if current is not None and colors[current] == 1:
            raise ValueError("parent cycle detected")
        for node in trail:
            colors[node] = 2

    roots = []
    for identity, node in nodes.items():
        parent = parents[identity]
        if parent is None:
            roots.append(node)
        else:
            nodes[parent]["children"].append(node)
    return roots


if __name__ == "__main__":
    rows = [{"id": 2, "parent_id": 1, "name": "child"}, {"id": 1, "parent_id": None, "name": "root"}]
    print(json.dumps(build_forest(rows), ensure_ascii=False))
