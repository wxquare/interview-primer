"""高并发优惠券分发的进程内练习：按 new/vip/regular 人群选择继承策略。
练习约定：整数分、有限库存、每个用户在本服务生命周期最多成功领取一张；重复请求返回原凭证。
策略决定候选券顺序，锁内执行查重/扣库存/登记；配置运行期间不变。单进程线程安全，不提供跨进程事务。
原题未给具体券规则/并发指标；本例不据此宣称生产吞吐或分布式能力。
领取 O(候选券数)，空间 O(券种数+成功领取用户数)。运行：python3 coupon_distribution.py
"""
from abc import ABC, abstractmethod
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
from threading import Lock
from types import MappingProxyType


@dataclass(frozen=True)
class User:
    user_id: str
    group: str


@dataclass(frozen=True)
class Coupon:
    coupon_id: str
    amount_cents: int
    min_spend_cents: int


@dataclass(frozen=True)
class Receipt:
    user_id: str
    coupon_id: str


class DistributionPolicy(ABC):
    @abstractmethod
    def candidates(self, user):
        """返回按优先级排列的券 ID；策略必须无副作用、可并发调用。"""
        raise NotImplementedError


class NewUserPolicy(DistributionPolicy):
    def candidates(self, user):
        return ("welcome", "standard")


class VipPolicy(DistributionPolicy):
    def candidates(self, user):
        return ("vip", "standard")


class RegularPolicy(DistributionPolicy):
    def candidates(self, user):
        return ("standard",)


class CouponService:
    def __init__(self, coupons, stock, policies=None):
        catalog = {}
        for coupon in coupons:
            if not isinstance(coupon, Coupon) or not isinstance(coupon.coupon_id, str) or not coupon.coupon_id:
                raise ValueError("invalid coupon")
            if coupon.coupon_id in catalog:
                raise ValueError("duplicate coupon id")
            if type(coupon.amount_cents) is not int or type(coupon.min_spend_cents) is not int:
                raise ValueError("money must be integer cents")
            # 本练习选择门槛金额不低于优惠金额。
            if coupon.amount_cents <= 0 or coupon.min_spend_cents < coupon.amount_cents:
                raise ValueError("invalid coupon amount/threshold")
            catalog[coupon.coupon_id] = coupon
        if set(stock) - set(catalog):
            raise ValueError("stock references unknown coupon")
        inventory = {identity: stock.get(identity, 0) for identity in catalog}
        if any(type(value) is not int or value < 0 for value in inventory.values()):
            raise ValueError("stock must be nonnegative integer")
        self.catalog = MappingProxyType(catalog)
        self._stock = inventory
        self._policies = MappingProxyType(dict(policies if policies is not None else {
            "new": NewUserPolicy(), "vip": VipPolicy(), "regular": RegularPolicy(),
        }))
        self._issued = {}
        self._lock = Lock()

    def claim(self, user):
        if not isinstance(user, User) or not isinstance(user.user_id, str) or not user.user_id or not isinstance(user.group, str):
            raise ValueError("invalid user")
        policy = self._policies.get(user.group)
        if policy is None:
            raise ValueError("unknown user group")
        candidates = tuple(policy.candidates(user))
        if any(identity not in self.catalog for identity in candidates):
            raise ValueError("policy references unknown coupon")
        with self._lock:
            if user.user_id in self._issued:
                return self._issued[user.user_id]
            for identity in candidates:
                if self._stock[identity] > 0:
                    receipt = Receipt(user.user_id, identity)
                    self._stock[identity] -= 1
                    self._issued[user.user_id] = receipt
                    return receipt
            return None  # 库存不足不记录成功领取；调用方可稍后重试。

    def inventory(self):
        with self._lock:
            return dict(self._stock)


if __name__ == "__main__":
    service = CouponService([
        Coupon("welcome", 500, 2000), Coupon("vip", 1000, 5000), Coupon("standard", 200, 1000),
    ], {"welcome": 2, "vip": 1, "standard": 3})
    users = [User("u1", "new"), User("u1", "new"), User("u2", "vip"), User("u3", "regular")]
    with ThreadPoolExecutor(max_workers=4) as executor:
        for receipt in executor.map(service.claim, users):
            print(receipt)
    print("remaining:", service.inventory())
