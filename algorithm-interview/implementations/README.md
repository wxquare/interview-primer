# 手撕代码与算法实现

## 导航

- [算法学习路线](../guides/roadmap/00-start-here.md)
- [按来源索引](../guides/indexes/problems-by-source.md) / [按主题索引](../guides/indexes/problems-by-topic.md) / [模板索引](../guides/indexes/templates-by-topic.md)
- 本目录保存与具体题目解法相对独立的算法、数据结构和面试手撕代码。

这里收录 LeetCode 与《剑指 Offer》之外的算法、数据结构实现，以及并发、设计模式、资源管理、字符串处理和限流等手撕代码练习。源码统一位于平铺的 [`src/`](src/) 目录，文件通常采用小写英文 `snake_case`，并用主题前缀表达用途；Java 文件按 `public class` 名称使用大驼峰。

## 整理规则

- 一个文件只承载一个独立题目，或同一算法领域的一组可协同复用实现。
- `合集`类文件保留多个紧密相关的变体，例如多种排序或最短路；它们不是互不相关题目的堆放。
- 已有独立实现时不再从旧合集重复拆出同名副本，例如 `cache_lru.cc`、`cache_lfu.cc`、`string_kmp.cc`、`trie.cc` 与 `union_find.cc`。

## 算法合集

| 文件 | 主题与包含内容 | 主要入口 / 核心类型 | 难度 |
| --- | --- | --- | --- |
| [sorting_algorithms_collection.cc](src/sorting_algorithms_collection.cc) | 排序算法合集：冒泡、插入、选择、希尔、快排、归并、基数、计数、桶与堆排序。 | `sorting_algorithms::*_sort` | ★★★ |
| [number_theory_algorithms.cc](src/number_theory_algorithms.cc) | 数论与随机算法：素数筛、因子、GCD/LCM、扩欧、快速幂、组合数、采样与洗牌。 | `number_theory::is_prime`、`extended_gcd`、`mod_power` | ★★★ |
| [string_algorithms_collection.cc](src/string_algorithms_collection.cc) | 字符串算法合集：KMP、Trie 与 AC 自动机。 | `kmp`、`Trie`、`acAutomaton` | ★★★★ |
| [range_query_algorithms.cc](src/range_query_algorithms.cc) | 区间更新与查询：差分数组、树状数组、动态开点线段树。 | `difference`、`FenwickTree`、`SegTree` | ★★★★ |
| [knapsack_dp.cc](src/knapsack_dp.cc) | 背包 DP：0/1、完全、多重、二进制优化与混合背包。 | `zeroOnePack`、`completePack`、`boundedKnapsack`、`mixPack` | ★★★★ |
| [graph_shortest_path_algorithms.cc](src/graph_shortest_path_algorithms.cc) | 最短路基础合集：无权 BFS、Dijkstra、Bellman-Ford、Floyd。 | `bfs`、`dijkstra`、`bellmanFord`、`floyd` | ★★★★ |
| [graph_shortest_path_variants.cc](src/graph_shortest_path_variants.cc) | 最短路的另一组实现，含负环检测。 | `bfs`、`dijkstra`、`bellmanFord`、`floyd` | ★★★★ |
| [lru_cache_design.cc](src/lru_cache_design.cc) | 旧版数据结构合集：LRU、LFU、哈希表与跳表；优先使用下方独立实现。 | `LRUCache`、`LFUCache`、`MyHashMap`、`SkipList` | ★★★★ |

## 独立数据结构与基础算法

| 文件 | 题目/用途 | 主入口或核心类型 | 难度 |
| --- | --- | --- | --- |
| [cache_lru.cc](src/cache_lru.cc) | LRU 缓存设计。 | `LRUCache` | ★★★ |
| [cache_lfu.cc](src/cache_lfu.cc) | LFU 缓存设计。 | `LFUCache` | ★★★★ |
| [union_find.cc](src/union_find.cc) | 并查集：连通性、合并与路径压缩。 | `UnionFind` | ★★ |
| [fenwick_tree.cc](src/fenwick_tree.cc) | 树状数组：单点更新、前缀和与区间和。 | `FenwickTree` | ★★★ |
| [segment_tree.cc](src/segment_tree.cc) | 动态开点线段树与懒标记。 | `SegTree` | ★★★★ |
| [persistent_segment_tree.cc](src/persistent_segment_tree.cc) | 主席树：区间第 k 小。 | `PersistentSegmentTree` | ★★★★★ |
| [sparse_table.cc](src/sparse_table.cc) | 静态区间最值查询。 | `SparseTableMin` | ★★★ |
| [monotonic_structures.cc](src/monotonic_structures.cc) | 单调栈与单调队列。 | `next_greater_index`、`sliding_window_maximum` | ★★★ |
| [binary_search.cc](src/binary_search.cc) | 递归与迭代二分查找。 | `binarySearch` | ★ |
| [binary_search_tree.cc](src/binary_search_tree.cc) | 二叉搜索树插入、查找与删除。 | `BinarySearchTree` | ★★★ |
| [trie.cc](src/trie.cc) | 前缀树实现。 | `Trie` | ★★★ |
| [tree_lowest_common_ancestor.cc](src/tree_lowest_common_ancestor.cc) | 倍增法最近公共祖先。 | `LowestCommonAncestor::query` | ★★★★ |
| [heavy_light_decomposition.cc](src/heavy_light_decomposition.cc) | 树链剖分。 | `HeavyLightDecomposition` | ★★★★★ |
| [linear_basis.cc](src/linear_basis.cc) | 异或线性基。 | `LinearBasis` | ★★★★ |

## 图论、字符串与动态规划专题

| 文件 | 题目/用途 | 主入口或核心类型 | 难度 |
| --- | --- | --- | --- |
| [graph_bridges.cc](src/graph_bridges.cc) | 求无向图桥与割点。 | `BridgeAndCutVertex` | ★★★★ |
| [tarjan.cc](src/tarjan.cc) | Tarjan 强连通分量。 | `tarjan` | ★★★★ |
| [graph_zero_one_bfs.cc](src/graph_zero_one_bfs.cc) | 0-1 BFS。 | `zero_one_bfs` | ★★★ |
| [graph_minimum_spanning_tree.cc](src/graph_minimum_spanning_tree.cc) | Kruskal 最小生成树。 | `kruskal` | ★★★ |
| [graph_topological_sort.cc](src/graph_topological_sort.cc) | Kahn 拓扑排序与环检测。 | `topologicalSort` | ★★★ |
| [graph_bipartite_matching.cc](src/graph_bipartite_matching.cc) | 二分图判定与匈牙利匹配。 | `isBipartite`、`maximumMatching` | ★★★★ |
| [graph_max_flow_dinic.cc](src/graph_max_flow_dinic.cc) | Dinic 最大流。 | `Dinic::add_edge`、`max_flow` | ★★★★★ |
| [min_cost_max_flow.cc](src/min_cost_max_flow.cc) | 最小费用最大流。 | `MinCostMaxFlow` | ★★★★★ |
| [traveling_salesman.cc](src/traveling_salesman.cc) | 状压 DP 求 TSP。 | `travelingSalesman` | ★★★★★ |
| [manacher.cc](src/manacher.cc) | 线性最长回文子串。 | `longest_palindromic_substring` | ★★★★ |
| [z_function.cc](src/z_function.cc) | Z 函数。 | `z_function` | ★★★ |
| [digit_dp.cc](src/digit_dp.cc) | 数位 DP。 | `digit_dp::count_stepping_numbers` | ★★★★ |
| [matrix_exponentiation.cc](src/matrix_exponentiation.cc) | 矩阵快速幂。 | `matrix_power` | ★★★ |

## 工程与手撕代码示例

| 文件 | 题目/用途 | 主入口或核心类型 | 难度 |
| --- | --- | --- | --- |
| [bounded_producer_consumer.cc](src/bounded_producer_consumer.cc) | C++ 有界生产者—消费者模型。 | `ProducerConsumer` | ★★★ |
| [c_string_operations.cc](src/c_string_operations.cc) | C 风格字符串的复制、移动、填充、拼接、比较和长度操作。 | `cstring_ops` | ★★★ |
| [concurrency_dining_philosophers.go](src/concurrency_dining_philosophers.go) | 哲学家进餐与死锁规避。 | `DiningPhilosophers` | ★★★★ |
| [concurrency_h2o.go](src/concurrency_h2o.go) | 并发协调生成 H₂O。 | `H2O` | ★★★ |
| [concurrency_producer_consumer.go](src/concurrency_producer_consumer.go) | Go 并发生产与消费。 | `main` | ★★★ |
| [concurrency_producer_consumer_sum.go](src/concurrency_producer_consumer_sum.go) | Go 并发生产与消费并汇总结果。 | `main` | ★★★ |
| [concurrency_turn_based_print.go](src/concurrency_turn_based_print.go) | 使用并发原语轮流打印。 | `main` | ★★★ |
| [concurrency_turn_based_string.go](src/concurrency_turn_based_string.go) | 使用并发原语轮流输出字符串。 | `main` | ★★★ |
| [concurrency_zero_even_odd.go](src/concurrency_zero_even_odd.go) | 并发交替输出零、偶数和奇数。 | `main` | ★★★ |
| [factory_pattern.cc](src/factory_pattern.cc) | 工厂模式示例。 | `Factory` | ★★ |
| [integer_conversion.cc](src/integer_conversion.cc) | 带越界检查的整数解析与 2–36 进制格式化。 | `parse`、`format` | ★★★ |
| [lua_token_limiter.py](src/lua_token_limiter.py) | Redis 令牌桶限流客户端。 | `TokenLimiter` | ★★★ |
| [lua_token_rate_limit.lua](src/lua_token_rate_limit.lua) | Redis Lua 令牌桶限流脚本。 | Redis 脚本入口 | ★★★ |
| [read_write_locker.cc](src/read_write_locker.cc) | 读写锁实现。 | `ReadWriteLock` | ★★★ |
| [singleton_pattern.cc](src/singleton_pattern.cc) | 线程安全单例。 | `Singleton` | ★★ |
| [string_raii.cc](src/string_raii.cc) | 自定义字符串的资源管理。 | `CString` | ★★★ |
| [string_raii_copy_move.cc](src/string_raii_copy_move.cc) | 自定义字符串的拷贝与移动语义练习。 | `CString` | ★★★ |
| [visitor_pattern.cc](src/visitor_pattern.cc) | 访问者模式示例。 | `Visitor` | ★★ |

## 独立题目

| 文件 | 题目含义 | 主要入口 | 难度 |
| --- | --- | --- | --- |
| [graph_shortest_path_visiting_all_nodes.cc](src/graph_shortest_path_visiting_all_nodes.cc) | 访问所有节点的最短路径（状态压缩 BFS）。 | `shortestPathLength` | ★★★★★ |
| [graph_a_star_eight_puzzle.cc](src/graph_a_star_eight_puzzle.cc) | 八数码 A* 搜索。 | `solvePuzzle` | ★★★★★ |
| [interview_valid_sudoku.cc](src/interview_valid_sudoku.cc) | 判断 9×9 数独是否合法。 | `valid_sudoku` | ★★ |

## 面经手撕与逻辑题单（70 题）

本题单按八类收录 70 道面经题：前六类 53 道手撕练习已有源码索引，另有 AI、训练与 GPU 实现 10 题，以及智力与逻辑 7 题。后两类目前收录题目与条件，不表示已有代码或完整题解。已有 LRU、Trie 和单例在原文件完善；深拷贝与浅拷贝共用一个对照实现。原始记录标注“仅题名”或“条件不全”的题目，明确区分原题条件与练习约定；练习约定不代表原面试题的完整要求。

### 1. 数据结构与缓存

| 题目 | 源码 | 关键点与练习约定 |
| --- | --- | --- |
| 实现 LRU 缓存 | [cache_lru.cc](src/cache_lru.cc) | 【哈希表 + 双向链表；int 键值，容量为零不缓存，get 未命中返回 -1】 |
| 实现可放入 HashSet 的类 | [HashSetKey.java](src/HashSetKey.java) | 【练习身份为 tenant + userId；不可变字段与一致的 equals/hashCode】 |
| 实现链表数据结构 | [linked_list.cc](src/linked_list.cc) | 【单向 int 链表；头尾插入、删除、反转与节点所有权】 |
| 实现前缀树 Trie | [trie.cc](src/trie.cc) | 【练习字符集为 a–z；完整单词标记、前缀查询与非法字符检查】 |
| 实现 HashMap | [hash_map_chaining.cc](src/hash_map_chaining.cc) | 【拉链法处理冲突；0.75 负载因子与重新散列】 |
| 使用 Go 实现 Set | [go_set.go](src/go_set.go) | 【map[T]struct{}；泛型 comparable 与零值可用的集合】 |
| 实现栈 | [stack_array.cc](src/stack_array.cc) | 【动态数组栈；几何扩容与空栈边界】 |
| 使用队列实现栈 | [stack_using_queues.cc](src/stack_using_queues.cc) | 【单队列入队后旋转；push O(n)、pop O(1)】 |
| 使用栈实现队列 | [queue_using_stacks.cc](src/queue_using_stacks.cc) | 【输入/输出双栈；只在输出栈为空时转移】 |
| 支持泛型和过期时间的 LRU 缓存 | [cache_lru_ttl.cc](src/cache_lru_ttl.cc) | 【泛型键值、每条记录 TTL；put 重置、get 不续期，先清过期再淘汰】 |
| 用数组实现 Map | [map_open_addressing.cc](src/map_open_addressing.cc) | 【数组 + 线性探测；墓碑删除、负载因子与扩容，无现成哈希表】 |

### 2. 并发、同步与异步

| 题目 | 源码 | 关键点与练习约定 |
| --- | --- | --- |
| 实现协程池的思路 | [concurrency_worker_pool.go](src/concurrency_worker_pool.go) | 【固定 worker + 有界队列；合作取消、关闭顺序与任务错误】 |
| 基于 Redis 实现每用户每分钟最多 100 次访问的限流器 | [redis_user_rate_limit.lua](src/redis_user_rate_limit.lua) | 【练习采用滑动 60 秒；TIME + ZSET + 原子 Lua，每用户最多 100 次放行】 |
| 使用 MySQL 实现可重入锁 | [MysqlReentrantLock.java](src/MysqlReentrantLock.java) | 【锁行持久化；owner token、重入计数与事务行锁；需要 DataSource/JDBC】 |
| 实现多生产者、多消费者消息队列 | [concurrency_mpmc_queue.go](src/concurrency_mpmc_queue.go) | 【手写环形缓冲区 + mutex/Cond；Close 唤醒等待者，消费者排空】 |
| 并发查询可用支付方式 | [concurrency_payment_queries.go](src/concurrency_payment_queries.go) | 【保留独立远程接口；有界并发、整体 deadline、可用结果和错误分开返回】 |
| 实现多线程计数器 | [concurrency_atomic_counter.cc](src/concurrency_atomic_counter.cc) | 【atomic fetch_add；纯计数使用 relaxed，join 后读取】 |
| Go 并发控制与一秒超时 | [concurrency_batch_timeout.go](src/concurrency_batch_timeout.go) | 【练习选整体 1 秒期限和最多 1000 个任务 worker；要求任务响应 context】 |
| 实现最多 10 个 goroutine 的并发执行器 | [concurrency_bounded_executor.go](src/concurrency_bounded_executor.go) | 【最多 10 个任务 worker；完成后立即领取下一项，不分批等待】 |
| 用两个 goroutine 生产并打印 5 个随机数 | [concurrency_random_numbers.go](src/concurrency_random_numbers.go) | 【恰好两个 worker；生产 5 个随机数、关闭 channel、消费打印】 |
| 两个线程交替打印 1–10 | [concurrency_alternate_numbers.cc](src/concurrency_alternate_numbers.cc) | 【条件变量 + 奇偶谓词；两个线程按序输出 1–10】 |
| 实现按 IP 每秒最多 10 次访问的本地限流器 | [rate_limit_ip.cc](src/rate_limit_ip.cc) | 【练习采用精确滑动 1 秒窗口；每 IP 最多 10 次放行，定期清理闲置键】 |
| 使用 AQS 实现可重入公平锁 | [AqsFairReentrantLock.java](src/AqsFairReentrantLock.java) | 【AQS state 保存重入次数；公平队列、持有者检查、Condition 与限时获取】 |
| 编写三个线程形成死锁的程序 | [concurrency_three_thread_deadlock.cc](src/concurrency_three_thread_deadlock.cc) | 【三锁环形等待 + 屏障；需显式 --deadlock，启用后不会自行结束】 |
| 三个 goroutine 顺序打印 123，共循环 100 次 | [concurrency_print_123.go](src/concurrency_print_123.go) | 【三个令牌通道传递打印权；123 循环 100 次，最后一轮停止传递】 |
| 线程安全的 LRU 缓存 | [cache_lru_thread_safe.cc](src/cache_lru_thread_safe.cc) | 【同一 mutex 保护哈希、链表移动和淘汰；返回值副本】 |
| 两个栈实现线程安全队列 | [queue_two_stacks_thread_safe.cc](src/queue_two_stacks_thread_safe.cc) | 【整次双栈转移持锁；非阻塞 try_pop，空队列返回 nullopt】 |

### 3. 语言机制、模式与代码阅读

| 题目 | 源码 | 关键点与练习约定 |
| --- | --- | --- |
| 实现深拷贝 | [CopySemantics.java](src/CopySemantics.java) | 【深拷贝 Person 的 Address 和 List；本练习模型不含循环引用】 |
| 实现浅拷贝 | [CopySemantics.java](src/CopySemantics.java) | 【浅拷贝复制顶层对象，保留嵌套 Address/List 引用，展示共享修改】 |
| 手写抽象工厂模式 | [factory_abstract.cc](src/factory_abstract.cc) | 【按钮/复选框两个产品抽象；Windows/Mac 产品族由同一抽象工厂创建】 |
| 手写单例模式 | [singleton_pattern.cc](src/singleton_pattern.cc) | 【C++11 局部静态初始化保证单例构造安全；禁用复制】 |
| 实现 StringBuilder | [InterviewStringBuilder.java](src/InterviewStringBuilder.java) | 【char[] 几何扩容；UTF-16 长度、null 追加语义、字符串快照】 |
| 手写策略模式 | [strategy_pattern.cc](src/strategy_pattern.cc) | 【继承实现满减/折扣策略；整数分、边界校验与避免乘法溢出】 |
| 实现动态代理 | [DynamicProxyExample.java](src/DynamicProxyExample.java) | 【JDK Proxy + InvocationHandler；处理 Object 方法，解包目标异常】 |
| 分析 Go 切片 append 与共享底层数组的输出 | [go_slice_append.go](src/go_slice_append.go) | 【初始 len=cap=3；第一次 append 共享，第二次扩容后分离】 |
| 比较 Integer 装箱对象与 int 的相等关系 | [IntegerEquality.java](src/IntegerEquality.java) | 【引用比较与拆箱比较；120 缓存保证，130 是否共享受运行时缓存配置影响】 |
| 手写 React Hook 的链表结构 | [react_hook_linked_list.js](src/react_hook_linked_list.js) | 【教学版 useState 节点/更新队列；按顺序消费 Hook，检测调用数量变化】 |

### 4. JSON、业务与状态处理

| 题目 | 源码 | 关键点与练习约定 |
| --- | --- | --- |
| 为字符串打码 | [string_mask.py](src/string_mask.py) | 【练习默认保留前 3 后 4 个 Unicode 码点；短串全遮蔽，参数可配置】 |
| 合并嵌套 JSON | [json_recursive_merge.py](src/json_recursive_merge.py) | 【递归合并且不修改输入；bool 先于数字，不同类型采用右值深拷贝】 |
| DFS 输出 JSON 对象叶子节点路径 | [json_leaf_paths.py](src/json_leaf_paths.py) | 【DFS 输出 JSON Pointer；数组用索引，空容器作为叶子，转义 ~ 和 /】 |
| 扁平 JSON 列表恢复成树形结构 | [json_flat_tree.py](src/json_flat_tree.py) | 【练习使用 id/parent_id；None 为根，保序，拒绝重复/缺父/环】 |
| 高并发优惠券分发系统 | [coupon_distribution.py](src/coupon_distribution.py) | 【继承的人群策略；锁内原子扣库存与用户幂等；进程内并发练习】 |
| 带缓存的多轮对话状态追踪器 | [conversation_state_cache.py](src/conversation_state_cache.py) | 【状态树 + LRU + 层级/派生视图失效；版本校验回填，命中率依赖流量】 |

### 5. SQL 编程

| 题目 | 源码 | 关键点与练习约定 |
| --- | --- | --- |
| SQL 查询总分大于 600 的学生及总分 | [sql_student_totals.sql](src/sql_student_totals.sql) | 【按 student_id 聚合 SUM(score)，HAVING 总分大于 600】 |
| SQL 查询订单数量最多的前十条广告 | [sql_top_ads.sql](src/sql_top_ads.sql) | 【LEFT JOIN + COUNT/COALESCE；同订单数按广告 ID，最多 10 行】 |
| SQL 查询人数前三的城市 | [sql_top_cities.sql](src/sql_top_cities.sql) | 【练习按城市人数排序；同人数按城市名，严格最多 3 行】 |
| SQL 查询 user 表中重复的 name | [sql_duplicate_names.sql](src/sql_duplicate_names.sql) | 【练习返回重复 name 与次数，排除 NULL；GROUP BY + HAVING】 |
| SQL 数据库行列转置 | [sql_pivot.sql](src/sql_pivot.sql) | 【自定学科成绩表；条件聚合行转列，CROSS JOIN + CASE 列转行】 |
| 构造 SQL 注入示例 | [sql_injection_demo.sql](src/sql_injection_demo.sql) | 【仅会话临时表展示拼接注入，再用 PREPARE 参数绑定修复】 |
| SQL 统计每个商家去重后的评论数 | [sql_merchant_reviews.sql](src/sql_merchant_reviews.sql) | 【练习以商家 + review_id 去重；保留零评论商家，忽略 NULL 评论 ID】 |

### 6. 脚本、文件与工具开发

| 题目 | 源码 | 关键点与练习约定 |
| --- | --- | --- |
| 实现用于调试任意 Pod 容器的 kubectl 插件 | [kubectl_debug_pod.sh](src/kubectl_debug_pod.sh) | 【封装 kubectl debug 的定向临时容器；Pod 网络共享、请求目标 PID namespace】 |
| Shell 统计访问次数最多的 10 个 IP | [shell_top_ips.sh](src/shell_top_ips.sh) | 【练习日志第 2 列是 IP；awk 计数、sort 降序、head 取前十】 |
| Shell 替换文件中的字符串 | [shell_replace_string.sh](src/shell_replace_string.sh) | 【练习按字面量替换；Python 标准库处理转义与原子文件替换】 |

### 7. AI、训练与 GPU 实现

按 GPU 归约、基础模型、注意力、训练监控和量化校准细分考点。以下题目尚未关联本目录实现；条件未记录的部分保留说明，不自行补成原题要求。

| 题目 | 细分类别 | 原题条件与关键点 |
| --- | --- | --- |
| 使用 Shared Memory 和多级 kernel 对大规模数组求和归约 | GPU 并行归约 | 【块内归约必须使用 Shared Memory，通过多级 kernel 完成全局汇总；与下方逐行归约题分别保留】 |
| 实现 K-means | 聚类算法 | 【条件不全：仅记录“手撕 kmeans”，未提供数据、初始化、距离度量或停止条件】 |
| 从 CSV 读取数据并实现简单 MLP | 数据读取与基础模型 | 【条件不全：需要读取 CSV 并实现简单 MLP；网络结构、任务、损失函数和训练要求未给出】 |
| 实现多头注意力 MHA | 注意力机制 | 【条件不全：仅记录“手撕 MHA”；张量形状、mask 和框架要求未给出】 |
| 奖励漂移检测伪代码 | 训练监控 | 【条件不全：用 KL 散度监控相邻两天的 Reward 输出分布并自动告警；阈值与分布估计条件未给出】 |
| 使用 PyTorch 编写 SFT 训练的 loss 计算逻辑 | 训练损失与标签对齐 | 【条件不全：原文特别要求注意标签的 shift right 对齐操作；张量接口等约束未给出，实现前需明确 logits 与目标标签的对应位置】 |
| 手写 GQA（Grouped Query Attention） | 注意力机制 | 【仅题名：要求实现 GQA，未记录具体形状、语言或接口】 |
| 实现 minmax 校准算法 | 量化校准 | 【仅题名：原文与 percentile 校准分别要求实现；接口及具体参数未记录，单独保留为一题】 |
| 实现 percentile 校准算法 | 量化校准 | 【仅题名：百分位参数、接口及具体要求未记录，不与 minmax 校准合并计数】 |
| 编写 CUDA 算子计算矩阵每一行的 reduce sum | GPU 并行归约 | 【归约维度为矩阵的每一行；未要求 Shared Memory 或多级 kernel，不混入大数组全局归约题的额外条件】 |

### 8. 智力与逻辑题

按状态搜索、信息编码、比较推理、概率、最坏情况决策和博弈分类；保留影响答案的条件缺口。

| 题目 | 细分类别 | 原题条件与关键点 |
| --- | --- | --- |
| 用 5L 和 3L 的桶，从无限量的水中量出 4L | 容器量取与状态搜索 | 【以两桶当前水量为状态，考虑装满、倒空和相互倾倒；目标量为 4L】 |
| 999 瓶水加 1 瓶毒药，用 10 只小白鼠在一周内找出毒瓶 | 信息编码 | 【共 1000 瓶；饮毒一周后死亡，只有一周观察时间；考察一次观察下的编码方案，不能安排死亡后再进行第二轮观察】 |
| 25 匹马、5 条跑道，最少比赛几场才能选出最快的 3 匹 | 比较排序与候选淘汰 | 【利用每场比赛提供的相对次序筛选前三候选；推导前需明确能否计时、速度是否稳定等练习前提】 |
| 根据抛掷结果，求取到正常硬币的概率：正常硬币正反不同，作弊硬币两面相同；连续 5 次出现同一面 | 概率与贝叶斯推断 | 【条件不全：袋中各一枚正常/作弊硬币，取一枚连续抛 5 次同面；取样先验及正常硬币公平性未明确，不能直接给出唯一数值概率】 |
| 用天平找出 7 个球中重量不同的球 | 称重与信息区分 | 【条件不全：未明确异球轻重是否预先已知；需分别讨论已知轻重和未知轻重的模型，不采纳原文存在问题的称重解法】 |
| 两个小球寻找摔碎的临界楼层 | 最坏情况决策 | 【条件不全：讨论总楼层 N 下的最坏尝试次数，例举 100 层；小球等价性、破碎单调性及临界层定义等前提需先明确】 |
| 20 个球轮流拿，每次拿 1、2、4 个，拿到最后一球者输 | 取球博弈 | 【区分必胜/必败状态，注意“拿到最后一球者输”的终局；要求判断先后手胜负并给出固定策略】 |

### 运行方式

已关联源码的文件按独立练习组织，含 `main` 的文件提供调用示例。以下命令从仓库根目录执行，按需要替换文件名；Go 目录包含多个 `main`，应指定单个文件。

```bash
mkdir -p .agents/build
# C++17；涉及线程的示例加 -pthread
g++ -std=c++17 -pthread algorithm-interview/implementations/src/cache_lru_thread_safe.cc -o .agents/build/lru
.agents/build/lru
# Go 1.18+（泛型 Set 需要），每次指定一个文件
go run algorithm-interview/implementations/src/concurrency_worker_pool.go
# Java 8+，类输出到构建目录
javac -d .agents/build algorithm-interview/implementations/src/AqsFairReentrantLock.java
java -cp .agents/build AqsFairReentrantLock
# Python 3.8+ / Node.js
python3 algorithm-interview/implementations/src/json_recursive_merge.py
node algorithm-interview/implementations/src/react_hook_linked_list.js
# MySQL 8：SQL 示例在当前连接创建临时表
mysql < algorithm-interview/implementations/src/sql_student_totals.sql
# Shell
sh algorithm-interview/implementations/src/shell_top_ips.sh access.log
```

Redis 限流脚本的调用方式在源码注释中；MySQL 可重入锁需由调用方提供配置好的 `DataSource` 和 JDBC 驱动。`kubectl_debug_pod.sh` 可安装为 `kubectl-debugpod` 插件，使用 [Kubernetes 官方临时容器机制](https://kubernetes.io/docs/tasks/debug/debug-application/debug-running-pod/)，需要对应集群权限和容器运行时支持。

优惠券示例的库存和幂等状态位于单进程内存；会话缓存示例中的重复读取流量用于观察命中率，实际是否达到 85% 需要用目标流量评估。三个线程死锁示例只有显式传入 `--deadlock` 才会进入永久等待。

## 后续拆分边界

当前无需把同一算法领域的合集强行拆成几十个小文件；这些函数共享辅助逻辑且作为复习模板更易查阅。若后续要进一步模块化，优先拆分 `lru_cache_design.cc` 中尚未独立的 `MyHashMap` 与 `SkipList`，再处理 `number_theory_algorithms.cc` 中的随机采样部分。
