-- 题目：构造 SQL 注入示例并给出参数化修复。MySQL 8；只操作当前连接临时演示数据。
-- 模型：拼接用户名到查询。输入为单引号 + OR 1=1 -- 后，改变条件结构，绕过剩余筛选。
-- 实际密码验证应由应用层安全密码哈希方案处理；下方字符串仅用于演示 SQL 语法，不是认证方案。
-- 无用户名索引时扫描 O(n)。运行：mysql < sql_injection_demo.sql
CREATE TEMPORARY TABLE demo_accounts (id BIGINT PRIMARY KEY, username VARCHAR(100), password_hash VARCHAR(100));
INSERT INTO demo_accounts VALUES (1,'alice','example_hash_a'),(2,'bob','example_hash_b');
-- 模拟不安全拼接后的查询；-- 后有空格，注释掉了原来的密码条件。
SELECT id FROM demo_accounts
WHERE username = '' OR 1=1 -- ' AND password_hash = 'wrong'
;
-- 参数化后输入被当成数据，不再能改变 SQL 结构。
SET @input_name = CONCAT(CHAR(39), ' OR 1=1 -- ');
SET @input_hash = 'wrong';
PREPARE safe_lookup FROM 'SELECT id FROM demo_accounts WHERE username = ? AND password_hash = ?';
EXECUTE safe_lookup USING @input_name, @input_hash;
DEALLOCATE PREPARE safe_lookup;
