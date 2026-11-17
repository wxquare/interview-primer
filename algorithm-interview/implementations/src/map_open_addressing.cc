// 题目：不用现成哈希表，用数组实现 Map。练习约定：int 键值。
// 线性探测、墓碑删除、负载因子约 0.7 扩容。平均 O(1)，最坏 O(n)，空间 O(容量)。
// 运行：g++ -std=c++17 map_open_addressing.cc -o map && ./map
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>
class ArrayMap {
    enum class State { Empty, Occupied, Deleted };
    struct Slot { int key=0,value=0; State state=State::Empty; };
    std::vector<Slot> slots_;
    std::size_t size_=0,used_=0; // used 含墓碑，防止删插后探测链无限增长。
    std::size_t locate(int key) const {
        auto start=std::hash<int>{}(key)%slots_.size();
        for (std::size_t step=0;step<slots_.size();++step) {
            auto pos=(start+step)%slots_.size();
            if (slots_[pos].state==State::Empty) break;
            if (slots_[pos].state==State::Occupied && slots_[pos].key==key) return pos;
        }
        return slots_.size();
    }
    void insert_new(int key,int value) {
        auto start=std::hash<int>{}(key)%slots_.size();
        for (std::size_t step=0;step<slots_.size();++step) {
            auto& slot=slots_[(start+step)%slots_.size()];
            if (slot.state!=State::Occupied) {
                if (slot.state==State::Empty) ++used_;
                slot={key,value,State::Occupied}; ++size_; return;
            }
        }
        throw std::logic_error("table full");
    }
    void rehash(std::size_t capacity) {
        ArrayMap fresh(capacity);
        for (const auto& slot:slots_) if (slot.state==State::Occupied) fresh.insert_new(slot.key,slot.value);
        slots_.swap(fresh.slots_); size_=fresh.size_; used_=fresh.used_;
    }
public:
    explicit ArrayMap(std::size_t capacity=8):slots_(capacity) {
        if (!capacity) throw std::invalid_argument("zero capacity");
    }
    void put(int key,int value) {
        auto pos=locate(key);
        if (pos!=slots_.size()) { slots_[pos].value=value; return; }
        if (static_cast<double>(used_+1)/slots_.size()>0.7) rehash(slots_.size()*2);
        insert_new(key,value);
    }
    std::optional<int> get(int key) const {
        auto pos=locate(key);
        return pos==slots_.size()?std::nullopt:std::optional<int>(slots_[pos].value);
    }
    bool erase(int key) {
        auto pos=locate(key); if (pos==slots_.size()) return false;
        slots_[pos].state=State::Deleted; --size_; return true;
    }
    std::size_t size() const { return size_; }
};
int main() { ArrayMap map; map.put(1,10); map.put(9,90); map.erase(1); std::cout<<map.get(9).value_or(-1)<<'\n'; }
