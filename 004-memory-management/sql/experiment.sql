CREATE EXTENSION experiment;

SELECT experiment_palloc();
SELECT experiment_memctx();

DROP EXTENSION experiment;