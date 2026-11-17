// 题目：支持泛型和过期时间的 LRU。练习约定：每条记录独立 TTL，put 重置，get 不续期。
// TTL 必须 >0；steady_clock 计时；读取惰性失效，写入先清除全部过期项再按 LRU 淘汰。
// get 平均 O(1)，put O(capacity)，空间 O(capacity)；不提供线程安全。
// 运行：g++ -std=c++17 cache_lru_ttl.cc -o ttl && ./ttl
#include <chrono>
#include <iostream>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
template<class Key,class Value>
class TtlLRUCache {
    using Clock=std::chrono::steady_clock;
    struct Entry { Key key; Value value; Clock::time_point expires; };
    using Items=std::list<Entry>;
    std::size_t capacity_;
    Items items_;
    std::unordered_map<Key,typename Items::iterator> index_;
    void purge_expired(Clock::time_point now) {
        for (auto it=items_.begin();it!=items_.end();) {
            if (it->expires<=now) { index_.erase(it->key); it=items_.erase(it); }
            else ++it;
        }
    }
public:
    explicit TtlLRUCache(std::size_t capacity):capacity_(capacity){}
    TtlLRUCache(const TtlLRUCache&)=delete;
    TtlLRUCache& operator=(const TtlLRUCache&)=delete;
    std::optional<Value> get(const Key& key) {
        auto it=index_.find(key);
        if (it==index_.end()) return std::nullopt;
        if (it->second->expires<=Clock::now()) {
            items_.erase(it->second); index_.erase(it); return std::nullopt;
        }
        items_.splice(items_.begin(),items_,it->second);
        return it->second->value; // 返回副本，不暴露节点生命周期。
    }
    void put(const Key& key,Value value,std::chrono::milliseconds ttl) {
        if (ttl.count()<=0) throw std::invalid_argument("TTL must be positive");
        auto now=Clock::now();
        auto remaining=std::chrono::duration_cast<std::chrono::milliseconds>(Clock::time_point::max()-now);
        if (ttl>remaining) throw std::invalid_argument("TTL too large");
        purge_expired(now);
        auto it=index_.find(key);
        if (it!=index_.end()) {
            it->second->value=std::move(value); it->second->expires=now+ttl;
            items_.splice(items_.begin(),items_,it->second); return;
        }
        if (!capacity_) return;
        items_.push_front({key,std::move(value),now+ttl});
        try { index_.emplace(key,items_.begin()); }
        catch (...) { items_.pop_front(); throw; }
        if (items_.size()>capacity_) { index_.erase(items_.back().key); items_.pop_back(); }
    }
};
int main() {
    TtlLRUCache<std::string,std::string> cache(2);
    cache.put("user:1","Alice",std::chrono::seconds(5));
    std::cout<<cache.get("user:1").value_or("missing")<<'\n';
}
