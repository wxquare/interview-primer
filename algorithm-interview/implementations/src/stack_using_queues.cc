// 题目：使用队列实现栈；单队列插入后旋转，新元素位于队首。
// push O(n)，pop/top O(1)，空间 O(n)。空栈抛异常。
// 运行：g++ -std=c++17 stack_using_queues.cc -o stack && ./stack
#include <iostream>
#include <queue>
#include <stdexcept>
class QueueStack {
    std::queue<int> queue_;
public:
    void push(int value) {
        queue_.push(value);
        for (std::size_t i=1,n=queue_.size();i<n;++i) { queue_.push(queue_.front()); queue_.pop(); }
    }
    int top() const { if (empty()) throw std::out_of_range("empty stack"); return queue_.front(); }
    int pop() { int value=top(); queue_.pop(); return value; }
    bool empty() const { return queue_.empty(); }
};
int main() { QueueStack stack; stack.push(1); stack.push(2); std::cout<<stack.pop()<<'\n'; }
