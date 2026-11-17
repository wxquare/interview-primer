// 题目：手写 StringBuilder。char[] 几何扩容；练习接口 append(String/char/int)、charAt、toString。
// null 字符串追加为 "null"；按 UTF-16 char 计长；非线程安全。
// 追加 k 个字符摊还 O(k)，单次扩容 O(n)，toString O(n)，空间 O(容量)。
// 运行：javac InterviewStringBuilder.java && java InterviewStringBuilder
import java.util.Arrays;
public final class InterviewStringBuilder {
    private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;
    private char[] buffer;
    private int length;
    public InterviewStringBuilder() { this(16); }
    public InterviewStringBuilder(int capacity) {
        if (capacity < 0) throw new IllegalArgumentException("negative capacity");
        buffer = new char[capacity];
    }
    private void ensureCapacity(int required) {
        if (required < 0 || required > MAX_ARRAY_SIZE) throw new OutOfMemoryError("string too large");
        if (required <= buffer.length) return;
        long doubled = Math.max(1L, (long) buffer.length * 2);
        int capacity = (int) Math.max(required, Math.min(doubled, MAX_ARRAY_SIZE));
        buffer = Arrays.copyOf(buffer, capacity);
    }
    public InterviewStringBuilder append(String text) {
        if (text == null) text = "null";
        int required = Math.addExact(length, text.length()); ensureCapacity(required);
        text.getChars(0, text.length(), buffer, length); length = required; return this;
    }
    public InterviewStringBuilder append(char value) {
        ensureCapacity(Math.addExact(length, 1)); buffer[length++] = value; return this;
    }
    public InterviewStringBuilder append(int value) { return append(Integer.toString(value)); }
    public int length() { return length; }
    public char charAt(int index) {
        if (index < 0 || index >= length) throw new IndexOutOfBoundsException();
        return buffer[index];
    }
    @Override public String toString() { return new String(buffer, 0, length); }
    public static void main(String[] args) {
        InterviewStringBuilder builder = new InterviewStringBuilder(0);
        String snapshot = builder.append("answer=").append(42).toString();
        builder.append('!'); System.out.println(snapshot); System.out.println(builder);
    }
}
