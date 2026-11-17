-- 题目：查询总分大于 600 的学生及总分。MySQL 8 练习。
-- 约定：一名学生每个学科一条有效成绩；字段 subject/student_id/score，分数非空。
-- 聚合通常需扫描 O(n)，分组空间 O(学生数)；执行计划受索引/引擎影响。
-- 示例仅建会话临时表。运行：mysql < sql_student_totals.sql
CREATE TEMPORARY TABLE student_scores (
    student_id BIGINT NOT NULL,
    subject VARCHAR(32) NOT NULL,
    score DECIMAL(6,2) NOT NULL,
    PRIMARY KEY (student_id, subject)
);
INSERT INTO student_scores VALUES (1,'math',320),(1,'english',310),(2,'math',280),(2,'english',300);
SELECT student_id, SUM(score) AS total_score
FROM student_scores
GROUP BY student_id
HAVING SUM(score) > 600
ORDER BY total_score DESC, student_id ASC;
