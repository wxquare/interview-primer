// 题目：单例模式。C++11 局部静态变量初始化线程安全；禁用复制/赋值。
// getInstance O(1)，空间 O(1)；单例内部的可变业务状态仍需额外同步。
// 运行：g++ -std=c++17 singleton_pattern.cc -o singleton && ./singleton
#include <iostream>
class Singleton {
    Singleton()=default;
    ~Singleton()=default;
public:
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
    static Singleton* getInstance() { static Singleton instance; return &instance; }
};
int main() { std::cout<<std::boolalpha<<(Singleton::getInstance()==Singleton::getInstance())<<'\n'; }
