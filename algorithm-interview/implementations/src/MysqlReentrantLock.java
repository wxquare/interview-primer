// 题目：MySQL 实现可重入锁。练习约定：同一锁对象 + 同一线程的 token 为持有者。
// 持久锁记录 + SELECT FOR UPDATE 串行化元数据更新；事务结束后业务逻辑无需占数据库连接。
// 重入计数记录在数据库，释放到 0 清空 owner；记录不删除，避免删除/重建引发竞争。
// 数据库索引操作 O(log n)，本地空间 O(使用过本对象的线程数)。需要 MySQL JDBC 驱动/DataSource。
// 无租约：进程崩溃可能遗留锁；自动恢复需另加租约和 fencing，不能直接按超时强行删锁。
// tryLock 是一次业务抢占尝试，底层行锁/I/O 仍可能等待，应配置连接/查询超时。
// 编译：javac MysqlReentrantLock.java；main 仅输出 DDL，不连接真实数据库。
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Objects;
import java.util.UUID;
import javax.sql.DataSource;

public final class MysqlReentrantLock {
    public static final String DDL = "CREATE TABLE interview_locks ("
        + "lock_name VARCHAR(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin PRIMARY KEY, "
        + "owner VARCHAR(36) NULL, hold_count INT NOT NULL DEFAULT 0) ENGINE=InnoDB";
    private final DataSource dataSource;
    private final String name;
    private final ThreadLocal<String> token = ThreadLocal.withInitial(() -> UUID.randomUUID().toString());
    public MysqlReentrantLock(DataSource dataSource, String name) {
        this.dataSource = Objects.requireNonNull(dataSource);
        this.name = Objects.requireNonNull(name);
        if (name.isEmpty() || name.length() > 191) throw new IllegalArgumentException("invalid lock name");
    }
    private interface Transaction<T> { T run(Connection connection) throws SQLException; }
    private <T> T transaction(Transaction<T> action) throws SQLException {
        try (Connection connection = dataSource.getConnection()) {
            boolean original = connection.getAutoCommit();
            connection.setAutoCommit(false);
            boolean ended = false;
            try {
                T result = action.run(connection);
                connection.commit(); ended = true;
                return result;
            } catch (SQLException | RuntimeException exception) {
                try { connection.rollback(); ended = true; }
                catch (SQLException rollbackError) { exception.addSuppressed(rollbackError); }
                throw exception;
            } finally {
                // 回滚失败时不切回 autoCommit，以免意外提交；连接池应淘汰故障连接。
                if (ended) connection.setAutoCommit(original);
            }
        }
    }
    public boolean tryLock() throws SQLException {
        final String current = token.get();
        return transaction(connection -> {
            try (PreparedStatement insert = connection.prepareStatement(
                    "INSERT IGNORE INTO interview_locks(lock_name) VALUES (?)")) {
                insert.setString(1, name); insert.executeUpdate();
            }
            try (PreparedStatement select = connection.prepareStatement(
                    "SELECT owner,hold_count FROM interview_locks WHERE lock_name=? FOR UPDATE")) {
                select.setString(1, name);
                try (ResultSet row = select.executeQuery()) {
                    if (!row.next()) throw new SQLException("lock row missing");
                    String owner = row.getString(1); int count = row.getInt(2);
                    if (owner != null && !owner.equals(current)) return false;
                    if (count == Integer.MAX_VALUE) throw new IllegalStateException("hold count overflow");
                    try (PreparedStatement update = connection.prepareStatement(
                            "UPDATE interview_locks SET owner=?,hold_count=? WHERE lock_name=?")) {
                        update.setString(1, current); update.setInt(2, count + 1); update.setString(3, name);
                        update.executeUpdate(); return true;
                    }
                }
            }
        });
    }
    public void unlock() throws SQLException {
        final String current = token.get();
        transaction(connection -> {
            try (PreparedStatement select = connection.prepareStatement(
                    "SELECT owner,hold_count FROM interview_locks WHERE lock_name=? FOR UPDATE")) {
                select.setString(1, name);
                try (ResultSet row = select.executeQuery()) {
                    if (!row.next() || !current.equals(row.getString(1)) || row.getInt(2) <= 0)
                        throw new IllegalMonitorStateException("current thread is not the owner");
                    int next = row.getInt(2) - 1;
                    try (PreparedStatement update = connection.prepareStatement(
                            "UPDATE interview_locks SET owner=?,hold_count=? WHERE lock_name=?")) {
                        update.setString(1, next == 0 ? null : current);
                        update.setInt(2, next); update.setString(3, name); update.executeUpdate();
                    }
                }
            }
            return null;
        });
    }
    public static void main(String[] args) { System.out.println(DDL + ";"); }
}
