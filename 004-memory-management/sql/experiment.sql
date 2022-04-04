CREATE EXTENSION experiment;

SELECT experiment_palloc();
SELECT experiment_ctxnames();
SELECT experiment_memctx();

\set ON_ERROR_STOP 0
SELECT experiment_tryfinally(fail => true);
\set ON_ERROR_STOP 1

SELECT experiment_tryfinally(fail => false);

DROP EXTENSION experiment;