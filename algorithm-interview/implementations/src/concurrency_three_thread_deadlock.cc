// 题目：三个线程形成死锁。每个线程先持有一把锁，同步后等待下一把，形成等待环。
// 默认只说明用法；显式 --deadlock 会永久阻塞，需要手动终止进程。
// 时间：死锁后不终止；空间 O(1)。
// 运行：g++ -std=c++17 -pthread concurrency_three_thread_deadlock.cc -o deadlock
//       ./deadlock --deadlock
#include <array>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
int main(int argc,char** argv) {
    if (argc!=2 || std::string(argv[1])!="--deadlock") {
        std::cout<<"Run with --deadlock to create the intentional circular wait.\n"; return 0;
    }
    std::array<std::mutex,3> resources;
    std::mutex barrier_mutex; std::condition_variable barrier; int acquired=0;
    auto work=[&](int i){
        std::lock_guard<std::mutex> first(resources[i]);
        {
            std::unique_lock<std::mutex> lock(barrier_mutex);
            ++acquired; barrier.notify_all();
            barrier.wait(lock,[&]{return acquired==3;});
        }
        std::lock_guard<std::mutex> second(resources[(i+1)%3]);
    };
    std::thread a(work,0),b(work,1),c(work,2); a.join(); b.join(); c.join();
}
