CREATE EXTENSION experiment;

SELECT experiment_elog();

\set ON_ERROR_STOP 0

SELECT experiment_ereport('hello');
SELECT experiment_trycatch();

\set ON_ERROR_STOP 1

DROP EXTENSION experiment;