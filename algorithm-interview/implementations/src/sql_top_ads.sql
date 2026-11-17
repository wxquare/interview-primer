-- 题目：订单最多的前十条广告，输出 ID、名称、订单数、费用总和。MySQL 8。
-- 约定：无订单广告也可参与；同订单数按广告 ID 排序，严格返回最多 10 行。
-- LEFT JOIN 防止遗漏零订单广告；COUNT(o.id) 不把 LEFT JOIN 空占位算成一笔订单。
-- 扫描/分组成本取决于数据量与索引，排序最坏 O(广告数 log 广告数)。
-- 运行：mysql < sql_top_ads.sql（仅创建临时表）
CREATE TEMPORARY TABLE ad (
    id BIGINT PRIMARY KEY, title VARCHAR(100) NOT NULL, create_time DATETIME NOT NULL
);
CREATE TEMPORARY TABLE `order` (
    id BIGINT PRIMARY KEY, ad_id BIGINT NOT NULL, cost DECIMAL(12,2) NOT NULL,
    create_time DATETIME NOT NULL, INDEX idx_ad_id(ad_id)
);
INSERT INTO ad VALUES (1,'ad A',NOW()),(2,'ad B',NOW());
INSERT INTO `order` VALUES (1,1,12.50,NOW()),(2,1,7.50,NOW());
SELECT a.id, a.title, COUNT(o.id) AS order_count, COALESCE(SUM(o.cost),0) AS total_cost
FROM ad AS a
LEFT JOIN `order` AS o ON o.ad_id = a.id
GROUP BY a.id, a.title
ORDER BY order_count DESC, a.id ASC
LIMIT 10;
