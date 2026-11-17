#!/bin/sh
# 题目：替换文件中的字符串。练习约定：UTF-8 参数按字面量替换全部非重叠匹配，不作正则解释。
# 拒绝空搜索串和符号链接；同目录临时文件 + 原子替换，保留权限位；不保留硬链接关系。
# 文件整体读入，时间/空间 O(文件大小+输出大小)；依赖 python3。
# 运行：sh shell_replace_string.sh FILE 'old text' 'new text'
set -eu
if [ "$#" -ne 3 ]; then
    printf 'Usage: %s FILE OLD NEW\n' "$0" >&2
    exit 2
fi
exec python3 - "$1" "$2" "$3" <<'PYTHON'
import os
from pathlib import Path
import stat
import sys
import tempfile

path = Path(sys.argv[1])
old, new = sys.argv[2].encode('utf-8'), sys.argv[3].encode('utf-8')
if not old:
    raise SystemExit('OLD must not be empty')
if path.is_symlink() or not path.is_file():
    raise SystemExit('FILE must be a regular non-symlink file')
content = path.read_bytes()
count = content.count(old)
if not count:
    print('0 replacements')
    raise SystemExit(0)
mode = stat.S_IMODE(path.stat().st_mode)
fd, temporary = tempfile.mkstemp(prefix='.replace-', dir=str(path.parent))
try:
    with os.fdopen(fd, 'wb') as output:
        output.write(content.replace(old, new))
        output.flush()
        os.fsync(output.fileno())
    os.chmod(temporary, mode)
    os.replace(temporary, path)
finally:
    if os.path.exists(temporary):
        os.unlink(temporary)
print(str(count) + ' replacements')
PYTHON
