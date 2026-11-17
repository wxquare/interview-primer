-- 题目：统计每商家去重评论数。MySQL 8；原题未给表结构/去重字段。
-- 练习约定：reviews.row_id 是采集行 ID，review_id 是业务评论 ID；同商家同 review_id 算一次。
-- NULL review_id 不计入；保留无评论商家，返回 0；商家表之外的孤立评论不进入结果。
-- 扫描/去重取决于执行计划，辅助空间 O(不同商家评论组合数)。运行：mysql < sql_merchant_reviews.sql
CREATE TEMPORARY TABLE merchants (id BIGINT PRIMARY KEY, name VARCHAR(100) NOT NULL);
CREATE TEMPORARY TABLE reviews (
    row_id BIGINT PRIMARY KEY, merchant_id BIGINT NOT NULL, review_id BIGINT,
    INDEX idx_merchant_review(merchant_id,review_id)
);
INSERT INTO merchants VALUES (1,'shop A'),(2,'shop B');
INSERT INTO reviews VALUES (1,1,101),(2,1,101),(3,1,102);
SELECT m.id, m.name, COALESCE(r.review_count,0) AS review_count
FROM merchants AS m
LEFT JOIN (
    SELECT merchant_id, COUNT(DISTINCT review_id) AS review_count
    FROM reviews
    GROUP BY merchant_id
) AS r ON r.merchant_id = m.id
ORDER BY m.id;
