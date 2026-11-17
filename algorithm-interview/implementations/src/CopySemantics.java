// 题目：浅拷贝与深拷贝。练习模型：Person 含可变 Address 和 List<String>。
// 浅拷贝共享引用；深拷贝重建可变对象，String 可共享。此模型不含循环或共享子图。
// 浅拷贝 O(1)，深拷贝 O(tags 数)，相同量级额外空间。
// 运行：javac CopySemantics.java && java CopySemantics
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public final class CopySemantics {
    static final class Address {
        String city;
        Address(String city) { this.city = city; }
    }
    static final class Person {
        String name; Address address; List<String> tags;
        Person(String name, Address address, List<String> tags) {
            this.name = name; this.address = address; this.tags = tags;
        }
        Person shallowCopy() { return new Person(name, address, tags); }
        Person deepCopy() {
            return new Person(name, address == null ? null : new Address(address.city),
                tags == null ? null : new ArrayList<>(tags));
        }
    }
    public static void main(String[] args) {
        Person original = new Person("Alice", new Address("Shanghai"), new ArrayList<>(Arrays.asList("new")));
        Person shallow = original.shallowCopy(); Person deep = original.deepCopy();
        shallow.address.city = "Beijing"; shallow.tags.add("shared");
        System.out.println("original=" + original.address.city + original.tags);
        System.out.println("deep=" + deep.address.city + deep.tags);
    }
}
