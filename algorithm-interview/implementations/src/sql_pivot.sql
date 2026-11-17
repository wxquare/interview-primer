-- 题目：行列转置。原题只有题名；练习自定 student_id/subject/score，固定 math/english 两科。
-- MySQL 8：条件聚合完成行转列，科目表 CROSS JOIN + CASE 完成列转行；缺失科目为 NULL，不把缺失等同零分。
-- 每学生每科唯一；额外学科不进入这份固定列示例。扫描 O(n)，分组空间 O(学生数)。
-- 运行：mysql < sql_pivot.sql（所有表为临时表）
CREATE TEMPORARY TABLE pivot_scores (
    student_id BIGINT NOT NULL, subject VARCHAR(32) NOT NULL, score INT NOT NULL,
    PRIMARY KEY (student_id, subject)
);
INSERT INTO pivot_scores VALUES (1,'math',90),(1,'english',85),(2,'math',70);
CREATE TEMPORARY TABLE wide_scores AS
SELECT student_id,
       MAX(CASE WHEN subject='math' THEN score END) AS math_score,
       MAX(CASE WHEN subject='english' THEN score END) AS english_score
FROM pivot_scores
GROUP BY student_id;
SELECT * FROM wide_scores ORDER BY student_id;
-- 再恢复成长表；科目集合写成两行，wide_scores 只引用一次。
SELECT w.student_id, subjects.subject,
       CASE subjects.subject WHEN 'math' THEN w.math_score ELSE w.english_score END AS score
FROM wide_scores AS w
CROSS JOIN (SELECT 'math' AS subject UNION ALL SELECT 'english') AS subjects
WHERE (subjects.subject='math' AND w.math_score IS NOT NULL)
   OR (subjects.subject='english' AND w.english_score IS NOT NULL)
ORDER BY w.student_id, subjects.subject;
