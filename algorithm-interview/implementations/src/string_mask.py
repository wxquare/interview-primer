"""为字符串打码。练习约定：保留前 3、后 4 个 Unicode 码点，中间用 * 替换。
长度不足以保留两端时全部打码；空串返回空串。参数可调整，不按字节或字素簇计数。
时间/空间 O(n)。运行：python3 string_mask.py
"""
def mask_string(text, prefix=3, suffix=4, mask="*"):
    if not isinstance(text, str) or not isinstance(mask, str) or len(mask) != 1:
        raise ValueError("text must be a string; mask must be one character")
    if not isinstance(prefix, int) or not isinstance(suffix, int) or prefix < 0 or suffix < 0:
        raise ValueError("prefix and suffix must be nonnegative integers")
    if len(text) <= prefix + suffix:
        return mask * len(text)
    tail = text[-suffix:] if suffix else ""
    return text[:prefix] + mask * (len(text) - prefix - suffix) + tail

if __name__ == "__main__":
    print(mask_string("13812345678"))
    print(mask_string("short"))
