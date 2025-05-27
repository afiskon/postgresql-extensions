CREATE EXTENSION experiment;

SELECT experiment_max(1, 2);
SELECT experiment_max(true, false);
SELECT experiment_max('aaa', 'bbb' :: text COLLATE "C");
SELECT experiment_max('{1,2,3}', '{4,5,6}' :: int[]);

DROP EXTENSION experiment;
