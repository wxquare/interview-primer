// 题目：比较 Integer 装箱对象与 int 的相等关系，120 与 130。
// Integer==Integer 比引用；Integer==int 拆箱比较值；equals 比数值（参数类型仍重要）。
// 常量装箱 120 命中保证的缓存范围；130 通常不缓存，但缓存上界可扩展，不写死 false。
// null 拆箱会抛 NPE。时间/空间 O(1)。运行：javac IntegerEquality.java && java IntegerEquality
public final class IntegerEquality {
    public static void main(String[] args) {
        Integer i1 = 120, i2 = 120; int i3 = 120;
        System.out.println("120 references: " + (i1 == i2));
        System.out.println("120 unboxed: " + (i1 == i3));
        Integer j1 = 130, j2 = 130; int j3 = 130;
        System.out.println("130 references (runtime cache dependent): " + (j1 == j2));
        System.out.println("130 unboxed: " + (j1 == j3));
        System.out.println("130 equals: " + j1.equals(j2));
    }
}
