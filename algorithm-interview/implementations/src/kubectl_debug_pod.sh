#!/bin/sh
# 题目：用于调试 Pod 容器的 kubectl 插件。练习采用现有 kubectl debug 创建临时调试容器。
# 同 Pod 共享网络，--target 请求进入目标容器的进程命名空间；需容器运行时支持及相应 RBAC。
# 不假设根文件系统/挂载命名空间相同；需查看目标文件时可按权限访问 /proc/<pid>/root。
# 这不是自建 CRI/nsenter 方案；依赖支持 ephemeral containers 的 Kubernetes/kubectl。
# 官方机制：https://kubernetes.io/docs/tasks/debug/debug-application/debug-running-pod/
# 本地编排 O(1)，启动/连接成本由集群决定；未对真实集群执行。
# 安装：install -m 755 kubectl_debug_pod.sh "$HOME/.local/bin/kubectl-debugpod"
# 使用：kubectl debugpod NAMESPACE POD TARGET_CONTAINER DEBUG_IMAGE [COMMAND [ARGS...]]
# 示例：kubectl debugpod default api-pod api my-registry/debug:tag /bin/sh
set -eu
if [ "$#" -lt 4 ]; then
    printf 'Usage: %s NAMESPACE POD TARGET_CONTAINER DEBUG_IMAGE [COMMAND [ARGS...]]\n' "$0" >&2
    exit 2
fi
namespace=$1
pod=$2
container=$3
image=$4
shift 4
if [ -z "$namespace" ] || [ -z "$container" ] || [ -z "$image" ]; then
    printf 'Namespace, target container and image must not be empty\n' >&2
    exit 2
fi
case "$pod" in ''|-*|*/*) printf 'Invalid Pod name\n' >&2; exit 2 ;; esac
command -v kubectl >/dev/null 2>&1 || { printf 'kubectl is required\n' >&2; exit 1; }
if [ "$#" -eq 0 ]; then set -- /bin/sh; fi
if [ -t 0 ] && [ -t 1 ]; then
    exec kubectl debug "$pod" --namespace="$namespace" --target="$container" --image="$image" --stdin --tty -- "$@"
else
    exec kubectl debug "$pod" --namespace="$namespace" --target="$container" --image="$image" --stdin -- "$@"
fi
