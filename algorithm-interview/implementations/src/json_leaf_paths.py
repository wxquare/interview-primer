"""DFS 输出 JSON 叶子路径。练习约定：采用 JSON Pointer，数组索引为路径段。
~ 转义为 ~0，/ 转义为 ~1；空对象/空数组视作叶子；根本身为叶子时路径为空字符串。
输入是无环 JSON 值。遍历 O(节点数 + 输出路径字符数)，辅助空间 O(树深度)。
运行：python3 json_leaf_paths.py
"""
import json


def leaf_paths(value):
    segments = []

    def visit(node):
        if isinstance(node, dict) and node:
            for key, child in node.items():
                if not isinstance(key, str):
                    raise TypeError("JSON object keys must be strings")
                segments.append(key.replace("~", "~0").replace("/", "~1"))
                yield from visit(child)
                segments.pop()
        elif isinstance(node, list) and node:
            for index, child in enumerate(node):
                segments.append(str(index))
                yield from visit(child)
                segments.pop()
        else:
            yield "/" + "/".join(segments) if segments else ""

    yield from visit(value)


if __name__ == "__main__":
    document = {"user": {"name": "Alice"}, "a/b": [1, {}, []]}
    for path in leaf_paths(document):
        print(json.dumps(path, ensure_ascii=False))
