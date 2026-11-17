-- 题目：Redis 实现每用户每分钟最多 100 次访问。练习约定：滑动 60 秒，统计放行次数。
-- ZSET 保存毫秒时间戳；Redis TIME 统一时间；Lua 原子清理、计数、写入，拒绝请求不入集合。
-- KEYS[1]=rl:{user42}:events，KEYS[2]=rl:{user42}:seq；Cluster 必须使用相同 hash tag。
-- 返回 {是否放行, 剩余额度, 最早可重试毫秒数}；时间 O(log n + 过期数)，每用户最多 100 条。
-- 依赖 Redis；未连接真实服务。运行示例：
-- redis-cli --eval redis_user_rate_limit.lua 'rl:{user42}:events' 'rl:{user42}:seq'
local limit = 100
local window = 60000
local server_time = redis.call('TIME')
local now = tonumber(server_time[1]) * 1000 + math.floor(tonumber(server_time[2]) / 1000)
redis.call('ZREMRANGEBYSCORE', KEYS[1], '-inf', now - window)
local count = redis.call('ZCARD', KEYS[1])
if count >= limit then
    local first = redis.call('ZRANGE', KEYS[1], 0, 0, 'WITHSCORES')
    local retry = math.max(0, tonumber(first[2]) + window - now)
    return {0, 0, retry}
end
local sequence = redis.call('INCR', KEYS[2])
local member = tostring(now) .. ':' .. tostring(sequence)
redis.call('ZADD', KEYS[1], now, member)
redis.call('PEXPIRE', KEYS[1], window)
redis.call('PEXPIRE', KEYS[2], window)
return {1, limit - count - 1, 0}
