// 题目：实现 HashMap。练习约定：int 键值，拉链法，负载因子阈值 0.75。
// 平均查询/插入/删除 O(1)，最坏 O(n)，扩容 O(n)，空间 O(n+桶数)。
// 运行：g++ -std=c++17 hash_map_chaining.cc -o map && ./map
#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>
class HashMap {
    using Bucket = std::list<std::pair<int,int>>;
    std::vector<Bucket> buckets_;
    std::size_t size_ = 0;
    std::size_t bucket(int key) const { return std::hash<int>{}(key) % buckets_.size(); }
    void rehash(std::size_t capacity) {
        std::vector<Bucket> fresh(capacity);
        for (const auto& entries : buckets_)
            for (const auto& entry : entries)
                fresh[std::hash<int>{}(entry.first)%capacity].push_back(entry);
        buckets_.swap(fresh);
    }
public:
    explicit HashMap(std::size_t capacity=8):buckets_(capacity) {
        if (!capacity) throw std::invalid_argument("zero bucket count");
    }
    void put(int key,int value) {
        for (auto& entry : buckets_[bucket(key)])
            if (entry.first == key) { entry.second = value; return; }
        if (static_cast<double>(size_+1)/buckets_.size() > 0.75) rehash(buckets_.size()*2);
        buckets_[bucket(key)].emplace_back(key,value); ++size_;
    }
    std::optional<int> get(int key) const {
        for (const auto& entry : buckets_[bucket(key)]) if (entry.first == key) return entry.second;
        return std::nullopt;
    }
    bool erase(int key) {
        auto& entries = buckets_[bucket(key)];
        for (auto it=entries.begin();it!=entries.end();++it)
            if (it->first == key) { entries.erase(it); --size_; return true; }
        return false;
    }
    std::size_t size() const { return size_; }
};
int main() { HashMap map; map.put(-1,5); map.put(7,9); map.put(-1,6); std::cout<<map.get(-1).value_or(0)<<'\n'; map.erase(7); }
