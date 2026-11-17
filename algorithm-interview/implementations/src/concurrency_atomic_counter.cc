// 题目：多线程计数器。练习约定：只计数、不用于发布其他共享数据。
// atomic fetch_add(relaxed) 保证不丢增量；join 后读取；每次递增 O(1)，额外空间 O(1)。
// 运行：g++ -std=c++17 -pthread concurrency_atomic_counter.cc -o counter && ./counter
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
int main() {
    std::atomic<long long> counter{0};
    std::vector<std::thread> threads;
    for (int i=0;i<4;++i) threads.emplace_back([&]{
        for (int j=0;j<10000;++j) counter.fetch_add(1,std::memory_order_relaxed);
    });
    for (auto& thread:threads) thread.join();
    std::cout<<counter.load(std::memory_order_relaxed)<<'\n';
}
