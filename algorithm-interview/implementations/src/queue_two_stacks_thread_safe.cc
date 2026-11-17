// 题目：两个栈实现线程安全队列。练习约定：非阻塞 try_pop，空队列返回 nullopt。
// mutex 保护两个栈及整次转移；push O(1)，pop 摊还 O(1)、最坏 O(n)，空间 O(n)。
// 运行：g++ -std=c++17 -pthread queue_two_stacks_thread_safe.cc -o queue && ./queue
#include <iostream>
#include <mutex>
#include <optional>
#include <stack>
#include <thread>
class ConcurrentStackQueue {
    std::stack<int> input_,output_;
    std::mutex mutex_;
public:
    void push(int value) { std::lock_guard<std::mutex> lock(mutex_); input_.push(value); }
    std::optional<int> try_pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (output_.empty()) while (!input_.empty()) { output_.push(input_.top()); input_.pop(); }
        if (output_.empty()) return std::nullopt;
        int value=output_.top(); output_.pop(); return value;
    }
};
int main() {
    ConcurrentStackQueue queue;
    std::thread a([&]{queue.push(1);}); std::thread b([&]{queue.push(2);});
    a.join(); b.join(); while (auto value=queue.try_pop()) std::cout<<*value<<'\n';
}
