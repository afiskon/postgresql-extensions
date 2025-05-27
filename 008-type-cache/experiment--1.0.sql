-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION experiment" to load this file. \quit

CREATE OR REPLACE FUNCTION experiment_max(a ANYELEMENT, b ANYELEMENT) RETURNS ANYELEMENT
AS 'MODULE_PATHNAME', 'experiment_max' LANGUAGE C IMMUTABLE PARALLEL SAFE STRICT;
