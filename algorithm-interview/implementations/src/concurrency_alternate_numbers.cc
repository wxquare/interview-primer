// 题目：两个线程交替打印 1–10。条件变量等待谓词，避免虚假唤醒/丢失通知。
// 时间 O(n)，共享状态空间 O(1)。
// 运行：g++ -std=c++17 -pthread concurrency_alternate_numbers.cc -o print && ./print
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
int main() {
    std::mutex mutex; std::condition_variable ready; int next=1;
    auto print=[&](int parity){
        for (;;) {
            std::unique_lock<std::mutex> lock(mutex);
            ready.wait(lock,[&]{return next>10 || next%2==parity;});
            if (next>10) return;
            std::cout<<next++<<'\n'; lock.unlock(); ready.notify_all();
        }
    };
    std::thread odd(print,1),even(print,0); odd.join(); even.join();
}
