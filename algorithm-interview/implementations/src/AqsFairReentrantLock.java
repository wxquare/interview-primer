// 题目：基于 AQS 实现可重入公平锁。state 为重入次数，exclusiveOwnerThread 为持有线程。
// 练习约定：tryLock 也检查等待队列；公平指获取顺序策略，不保证操作系统调度顺序。
// 支持 Lock、可中断获取、限时获取与 Condition；不含序列化恢复。
// 无竞争获取/释放 O(1)，等待时间取决于竞争，等待队列空间 O(等待线程数)。
// 运行：javac AqsFairReentrantLock.java && java AqsFairReentrantLock
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.AbstractQueuedSynchronizer;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;

public final class AqsFairReentrantLock implements Lock {
    private static final class Sync extends AbstractQueuedSynchronizer {
        private static final long serialVersionUID = 1L;
        @Override protected boolean tryAcquire(int acquires) {
            Thread current = Thread.currentThread(); int state = getState();
            if (state == 0) {
                if (!hasQueuedPredecessors() && compareAndSetState(0, acquires)) {
                    setExclusiveOwnerThread(current); return true;
                }
            } else if (getExclusiveOwnerThread() == current) {
                int next = state + acquires;
                if (next < 0) throw new Error("hold count overflow");
                setState(next); return true;
            }
            return false;
        }
        @Override protected boolean tryRelease(int releases) {
            if (getExclusiveOwnerThread() != Thread.currentThread())
                throw new IllegalMonitorStateException("not lock owner");
            int next = getState() - releases;
            if (next < 0) throw new IllegalMonitorStateException("unbalanced unlock");
            boolean free = next == 0;
            if (free) setExclusiveOwnerThread(null);
            setState(next); return free;
        }
        @Override protected boolean isHeldExclusively() {
            return getExclusiveOwnerThread() == Thread.currentThread();
        }
        Condition condition() { return new ConditionObject(); }
    }
    private final Sync sync = new Sync();
    @Override public void lock() { sync.acquire(1); }
    @Override public void lockInterruptibly() throws InterruptedException { sync.acquireInterruptibly(1); }
    @Override public boolean tryLock() { return sync.tryAcquire(1); }
    @Override public boolean tryLock(long time, TimeUnit unit) throws InterruptedException {
        return sync.tryAcquireNanos(1, unit.toNanos(time));
    }
    @Override public void unlock() { sync.release(1); }
    @Override public Condition newCondition() { return sync.condition(); }
    public static void main(String[] args) {
        Lock lock = new AqsFairReentrantLock();
        lock.lock();
        try { lock.lock(); try { System.out.println("reentered"); } finally { lock.unlock(); } }
        finally { lock.unlock(); }
    }
}
