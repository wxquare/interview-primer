// 题目：线程安全的 LRU。练习约定：int 键值，未命中返回 optional 空值。
// 一把 mutex 覆盖哈希定位、链表移动和淘汰；返回值副本，无悬空引用。
// get/put 平均 O(1)（不含等待锁时间），空间 O(capacity)。
// 运行：g++ -std=c++17 -pthread cache_lru_thread_safe.cc -o lru && ./lru
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>
#include <utility>
class ThreadSafeLRU {
    using Items=std::list<std::pair<int,int>>;
    const std::size_t capacity_;
    Items items_;
    std::unordered_map<int,Items::iterator> index_;
    std::mutex mutex_;
public:
    explicit ThreadSafeLRU(std::size_t capacity):capacity_(capacity){}
    std::optional<int> get(int key) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it=index_.find(key);
        if (it==index_.end()) return std::nullopt;
        items_.splice(items_.begin(),items_,it->second); return it->second->second;
    }
    void put(int key,int value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it=index_.find(key);
        if (it!=index_.end()) {
            it->second->second=value; items_.splice(items_.begin(),items_,it->second); return;
        }
        if (!capacity_) return;
        items_.emplace_front(key,value);
        try { index_.emplace(key,items_.begin()); }
        catch (...) { items_.pop_front(); throw; }
        if (items_.size()>capacity_) { index_.erase(items_.back().first); items_.pop_back(); }
    }
};
int main() {
    ThreadSafeLRU cache(4);
    std::thread a([&]{cache.put(1,10);});
    std::thread b([&]{cache.put(2,20);});
    a.join(); b.join(); std::cout<<cache.get(1).value_or(-1)<<'\n';
}
