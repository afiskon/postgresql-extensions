CREATE EXTENSION experiment;

SELECT experiment_sum('{}' :: int[]);
SELECT experiment_sum('{NULL}' :: int[]);
SELECT experiment_sum('{1}' :: int[]);
SELECT experiment_sum('{1,2,3}' :: int[]);
SELECT experiment_sum('{1,NULL,3}' :: int[]);
SELECT experiment_sum('[-1:1]={1,2,3}'::int[]);
SELECT experiment_sum('{{1,2,3},{4,5,6}}'::int[]); -- error

SELECT experiment_max('{}' :: int[]);
SELECT experiment_max('{NULL}' :: int[]);
SELECT experiment_max('{1}' :: int[]);
SELECT experiment_max('{1,3,2}' :: int[]);
SELECT experiment_max('{1,NULL,3}' :: int[]);
SELECT experiment_max('[-1:1]={3,2,1}'::int[]);

SELECT experiment_max('{}' :: text[] COLLATE "C");
SELECT experiment_max('{NULL}' :: text[] COLLATE "C");
SELECT experiment_max('{"a"}' :: text[] COLLATE "C");
SELECT experiment_max('{"aa","cc","bb"}' :: text[] COLLATE "C");
SELECT experiment_max('{"a",NULL,"c"}' :: text[] COLLATE "C");
SELECT experiment_max('[-1:1]={"cc","bb","aa"}'::text[] COLLATE "C");
SELECT experiment_max('{{1,2,3},{4,5,6}}'::int[]); -- error

SELECT experiment_repeat(1, 0);
SELECT experiment_repeat(1, 3);
SELECT experiment_repeat('hello' :: text, 3);
SELECT experiment_repeat(1, -1); -- error

DROP EXTENSION experiment;
