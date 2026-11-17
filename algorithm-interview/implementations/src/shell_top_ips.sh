#!/bin/sh
# 题目：统计访问次数最多的 10 个 IP。练习输入：空白分隔的时间、IP、访问信息三列，无表头。
# IP 为第 2 列（可为 IPv6 字符串）；同次数按 IP 字典序；空行/不足三列忽略。
# 时间 O(n + u log u)，空间 O(u)，u 为不同 IP 数。
# 运行：sh shell_top_ips.sh access.log；不传文件或传 - 时读 stdin。
set -eu
if [ "$#" -gt 1 ]; then
    printf 'Usage: %s [FILE|-]\n' "$0" >&2
    exit 2
fi
input=${1:--}
case "$input" in -|/*) ;; *) input="./$input" ;; esac
if [ "$input" != - ] && [ ! -r "$input" ]; then
    printf 'Cannot read input file\n' >&2
    exit 1
fi
awk 'NF >= 3 { counts[$2]++ } END { for (ip in counts) printf "%d %s\n", counts[ip], ip }' "$input" |
    LC_ALL=C sort -k1,1nr -k2,2 |
    head -n 10
