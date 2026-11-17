"""递归合并 JSON，不修改输入。
练习约定：dict 按键递归，list 拼接，数字相加，str 拼接，bool 逻辑或；
不同类型或 None 冲突以右侧深拷贝覆盖。bool 必须先于数字判断，因为 bool 是 int 子类。
输入为无环 JSON 值，浮点数必须有限。时间/结果空间 O(输入总大小)，递归栈 O(深度)。
运行：python3 json_recursive_merge.py
"""
from copy import deepcopy
import json
import math


def _validate(value):
    if value is None or type(value) in (str, bool, int):
        return
    if type(value) is float and math.isfinite(value):
        return
    if type(value) is list:
        for child in value:
            _validate(child)
        return
    if type(value) is dict:
        for key, child in value.items():
            if type(key) is not str:
                raise TypeError("JSON object keys must be strings")
            _validate(child)
        return
    raise TypeError("expected a finite JSON value")


def merge_json(left, right):
    _validate(left)
    _validate(right)

    def merge(a, b):
        if type(a) is dict and type(b) is dict:
            result = {key: deepcopy(value) for key, value in a.items() if key not in b}
            for key, value in b.items():
                result[key] = merge(a[key], value) if key in a else deepcopy(value)
            return result
        if type(a) is list and type(b) is list:
            return deepcopy(a) + deepcopy(b)
        if type(a) is bool and type(b) is bool:
            return a or b
        if type(a) in (int, float) and type(b) in (int, float):
            value = a + b
            if type(value) is float and not math.isfinite(value):
                raise ValueError("numeric merge overflowed JSON finite range")
            return value
        if type(a) is str and type(b) is str:
            return a + b
        return deepcopy(b)

    return merge(left, right)


if __name__ == "__main__":
    a = {"profile": {"score": 2, "tags": ["new"], "active": False}, "name": "A"}
    b = {"profile": {"score": 3.5, "tags": ["vip"], "active": True}, "name": "B"}
    print(json.dumps(merge_json(a, b), ensure_ascii=False, allow_nan=False))
