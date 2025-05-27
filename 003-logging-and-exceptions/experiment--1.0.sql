-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION experiment" to load this file. \quit

CREATE OR REPLACE FUNCTION experiment_elog() RETURNS VOID
AS 'MODULE_PATHNAME', 'experiment_elog' LANGUAGE C IMMUTABLE PARALLEL SAFE STRICT;

CREATE OR REPLACE FUNCTION experiment_ereport(arg TEXT) RETURNS VOID
AS 'MODULE_PATHNAME', 'experiment_ereport' LANGUAGE C IMMUTABLE PARALLEL SAFE STRICT;

CREATE OR REPLACE FUNCTION experiment_trycatch() RETURNS VOID
AS 'MODULE_PATHNAME', 'experiment_trycatch' LANGUAGE C IMMUTABLE PARALLEL SAFE STRICT;
