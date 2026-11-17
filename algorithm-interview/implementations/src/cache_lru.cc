// 题目：实现 LRU 缓存。练习约定：int 键值；get 未命中返回 -1；容量为零不缓存。
// 哈希表定位 + 双向链表；get/put 平均 O(1)，空间 O(capacity)。
// 运行：g++ -std=c++17 cache_lru.cc -o lru && ./lru
#include <iostream>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>
class LRUCache {
    using Items = std::list<std::pair<int,int>>;
    std::size_t capacity_;
    Items items_;
    std::unordered_map<int, Items::iterator> index_;
public:
    explicit LRUCache(int capacity) : capacity_(0) {
        if (capacity < 0) throw std::invalid_argument("negative capacity");
        capacity_ = static_cast<std::size_t>(capacity);
    }
    LRUCache(const LRUCache&) = delete; // 默认复制会使索引指向原链表。
    LRUCache& operator=(const LRUCache&) = delete;
    int get(int key) {
        auto it = index_.find(key);
        if (it == index_.end()) return -1;
        items_.splice(items_.begin(), items_, it->second);
        return it->second->second;
    }
    void put(int key, int value) {
        auto it = index_.find(key);
        if (it != index_.end()) {
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second); return;
        }
        if (!capacity_) return;
        items_.emplace_front(key,value);
        try { index_.emplace(key, items_.begin()); }
        catch (...) { items_.pop_front(); throw; }
        if (items_.size() > capacity_) {
            index_.erase(items_.back().first); items_.pop_back();
        }
    }
    void print() const {
        for (const auto& [key,value] : items_) std::cout << key << ':' << value << ' ';
        std::cout << '\n';
    }
};
using LRU = LRUCache;
int main() {
    LRUCache cache(2); cache.put(1,10); cache.put(2,20);
    std::cout << cache.get(1) << '\n'; cache.put(3,30);
    std::cout << cache.get(2) << '\n'; cache.print();
}
