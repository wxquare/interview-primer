-- 题目：user 表中重复 name。MySQL 8；练习返回重复姓名及次数，不返回全部用户行。
-- 排除 NULL，字符串相等遵循列的 collation；如需全部重复行，可把聚合结果再 JOIN 回 user。
-- 扫描/分组通常 O(n)，空间 O(不同姓名数)。运行：mysql < sql_duplicate_names.sql
CREATE TEMPORARY TABLE `user` (id BIGINT PRIMARY KEY, name VARCHAR(100));
INSERT INTO `user` VALUES (1,'Alice'),(2,'Alice'),(3,'Bob'),(4,NULL);
SELECT name, COUNT(*) AS occurrences
FROM `user`
WHERE name IS NOT NULL
GROUP BY name
HAVING COUNT(*) > 1
ORDER BY occurrences DESC, name ASC;
