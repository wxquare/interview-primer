// 题目：每个 IP 每秒最多放行 10 次。练习约定：精确滑动窗口，窗口 (now-1s,now]。
// 只统计已放行请求；IP 作为非空字符串键；steady_clock 抵抗系统时间调整。
// 单 IP 最多 10 条时间戳；allow 平均 O(1)，空间 O(活跃/尚未清理的 IP 数)。
// cleanup_idle 由调用方定期调用，O(IP 数)，回收不再访问的 IP。
// 运行：g++ -std=c++17 -pthread rate_limit_ip.cc -o limiter && ./limiter
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
class IpRateLimiter {
    using Clock=std::chrono::steady_clock;
    std::unordered_map<std::string,std::deque<Clock::time_point>> requests_;
    std::mutex mutex_;
public:
    bool allow(const std::string& ip) {
        if (ip.empty()) throw std::invalid_argument("empty IP");
        std::lock_guard<std::mutex> lock(mutex_);
        auto now=Clock::now(); auto& times=requests_[ip];
        while (!times.empty() && times.front()<=now-std::chrono::seconds(1)) times.pop_front();
        if (times.size()>=10) return false;
        times.push_back(now); return true;
    }
    void cleanup_idle() {
        std::lock_guard<std::mutex> lock(mutex_); auto cutoff=Clock::now()-std::chrono::seconds(1);
        for (auto it=requests_.begin();it!=requests_.end();) {
            if (it->second.empty() || it->second.back()<=cutoff) it=requests_.erase(it); else ++it;
        }
    }
};
int main() {
    IpRateLimiter limiter;
    for (int i=0;i<11;++i) std::cout<<std::boolalpha<<limiter.allow("127.0.0.1")<<'\n';
}
