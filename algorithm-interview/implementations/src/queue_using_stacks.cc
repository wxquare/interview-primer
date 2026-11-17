// 题目：两个栈实现队列；仅当输出栈为空时转移输入栈。
// 入队 O(1)，出队/取首摊还 O(1)、最坏 O(n)，空间 O(n)，空队列抛异常。
// 运行：g++ -std=c++17 queue_using_stacks.cc -o queue && ./queue
#include <iostream>
#include <stack>
#include <stdexcept>
class StackQueue {
    std::stack<int> input_,output_;
    void prepare() { if (output_.empty()) while (!input_.empty()) { output_.push(input_.top()); input_.pop(); } }
public:
    void push(int value) { input_.push(value); }
    int front() { prepare(); if (output_.empty()) throw std::out_of_range("empty queue"); return output_.top(); }
    int pop() { int value=front(); output_.pop(); return value; }
    bool empty() const { return input_.empty()&&output_.empty(); }
};
int main() { StackQueue queue; queue.push(1); queue.push(2); std::cout<<queue.pop()<<'\n'; }
