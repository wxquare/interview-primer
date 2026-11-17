-- 题目：查询人数前三的城市。MySQL 8；原文未给并列处理。
-- 练习约定：每行代表一人，不按姓名去重，排除 NULL 城市；并列按城市名，最多 3 行。
-- 若要求“前三个名次且并列全保留”，应改为 DENSE_RANK() OVER (ORDER BY COUNT(*) DESC)。
-- 分组扫描 O(n)，城市排序 O(c log c)。运行：mysql < sql_top_cities.sql
CREATE TEMPORARY TABLE city_people (name VARCHAR(100) NOT NULL, city VARCHAR(100));
INSERT INTO city_people VALUES ('A','Shanghai'),('B','Shanghai'),('C','Beijing'),('D','Shenzhen');
SELECT city, COUNT(*) AS people_count
FROM city_people
WHERE city IS NOT NULL
GROUP BY city
ORDER BY people_count DESC, city ASC
LIMIT 3;
