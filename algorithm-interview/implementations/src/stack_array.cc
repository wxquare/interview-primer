// 题目：实现栈。int 动态数组；push 摊还 O(1)，pop/top O(1)，空间 O(容量)。
// 练习约定：空栈访问抛异常。运行：g++ -std=c++17 stack_array.cc -o stack && ./stack
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
class ArrayStack {
    std::unique_ptr<int[]> data_=std::make_unique<int[]>(4);
    std::size_t size_=0,capacity_=4;
public:
    void push(int value) {
        if (size_==capacity_) {
            auto fresh=std::make_unique<int[]>(capacity_*2);
            std::copy_n(data_.get(),size_,fresh.get()); data_.swap(fresh); capacity_*=2;
        }
        data_[size_++]=value;
    }
    int top() const { if (empty()) throw std::out_of_range("empty stack"); return data_[size_-1]; }
    int pop() { int value=top(); --size_; return value; }
    bool empty() const { return size_==0; }
    std::size_t size() const { return size_; }
};
int main() { ArrayStack stack; stack.push(1); stack.push(2); std::cout<<stack.pop()<<'\n'; }
