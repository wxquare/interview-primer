// 题目：实现可放入 HashSet 的类。练习约定：tenant + userId 为身份，displayName 不参与去重。
// 身份字段不可变；equals/hashCode 使用同一组字段，避免入集合后修改身份导致查找失败。
// HashSet 增删查平均 O(1)，空间 O(n)。运行：javac HashSetKey.java && java HashSetKey
import java.util.HashSet;
import java.util.Objects;
import java.util.Set;

public final class HashSetKey {
    private final String tenant;
    private final long userId;
    private final String displayName;
    public HashSetKey(String tenant, long userId, String displayName) {
        this.tenant = Objects.requireNonNull(tenant);
        this.userId = userId;
        this.displayName = Objects.requireNonNull(displayName);
    }
    @Override public boolean equals(Object other) {
        if (this == other) return true;
        if (!(other instanceof HashSetKey)) return false;
        HashSetKey key = (HashSetKey) other;
        return userId == key.userId && tenant.equals(key.tenant);
    }
    @Override public int hashCode() { return Objects.hash(tenant, userId); }
    @Override public String toString() { return tenant + ":" + userId + ":" + displayName; }
    public static void main(String[] args) {
        Set<HashSetKey> users = new HashSet<>();
        users.add(new HashSetKey("shop", 1, "Alice"));
        users.add(new HashSetKey("shop", 1, "Alice renamed"));
        System.out.println(users.size());
    }
}
