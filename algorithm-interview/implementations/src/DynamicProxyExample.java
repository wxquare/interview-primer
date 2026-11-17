// 题目：实现动态代理。JDK Proxy 代理接口；InvocationHandler 添加调用前后逻辑。
// 练习约定：Object 方法采用代理对象身份语义；反射异常解包，保留业务异常。
// 每次代理调度 O(1)，加上被代理方法成本；空间 O(接口/方法元数据)。
// 运行：javac DynamicProxyExample.java && java DynamicProxyExample
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Proxy;
public final class DynamicProxyExample {
    public interface Greeting { String hello(String name); }
    static final class RealGreeting implements Greeting {
        @Override public String hello(String name) { return "Hello " + name; }
    }
    public static Greeting wrap(Greeting target) {
        if (target == null) throw new NullPointerException("target");
        return (Greeting) Proxy.newProxyInstance(Greeting.class.getClassLoader(),
            new Class<?>[]{Greeting.class}, (proxy, method, args) -> {
                if (method.getDeclaringClass() == Object.class) {
                    switch (method.getName()) {
                        case "equals": return proxy == args[0];
                        case "hashCode": return System.identityHashCode(proxy);
                        case "toString": return "GreetingProxy@" + Integer.toHexString(System.identityHashCode(proxy));
                        default: throw new UnsupportedOperationException(method.getName());
                    }
                }
                System.out.println("before " + method.getName());
                try { return method.invoke(target, args); }
                catch (InvocationTargetException exception) { throw exception.getCause(); }
                finally { System.out.println("after " + method.getName()); }
            });
    }
    public static void main(String[] args) { System.out.println(wrap(new RealGreeting()).hello("Alice")); }
}
