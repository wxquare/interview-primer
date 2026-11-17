// 题目：策略模式。练习约定：订单价格为非负整数分；满减与折扣策略可替换。
// 折扣用万分比，优惠金额向下取整，最终金额不小于零；计算 O(1)，空间 O(1)。
// 运行：g++ -std=c++17 strategy_pattern.cc -o strategy && ./strategy
#include <algorithm>
#include <iostream>
#include <stdexcept>
struct PricingStrategy {
    virtual ~PricingStrategy()=default;
    virtual long long discount(long long cents) const=0;
};
class FlatDiscount:public PricingStrategy {
    long long threshold_,reduction_;
public:
    FlatDiscount(long long threshold,long long reduction):threshold_(threshold),reduction_(reduction) {
        if (threshold<0 || reduction<0) throw std::invalid_argument("negative rule");
    }
    long long discount(long long cents) const override { return cents>=threshold_?std::min(cents,reduction_):0; }
};
class RateDiscount:public PricingStrategy {
    int basis_points_;
public:
    explicit RateDiscount(int points):basis_points_(points) {
        if (points<0 || points>10000) throw std::invalid_argument("discount outside 0..10000");
    }
    long long discount(long long cents) const override {
        // 分段计算，避免 cents*basis_points 溢出。
        return cents/10000*basis_points_+(cents%10000)*basis_points_/10000;
    }
};
long long checkout(long long cents,const PricingStrategy& strategy) {
    if (cents<0) throw std::invalid_argument("negative price");
    return cents-strategy.discount(cents);
}
int main() { FlatDiscount flat(10000,2000); RateDiscount rate(1500); std::cout<<checkout(12000,flat)<<' '<<checkout(12000,rate)<<'\n'; }
