-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION experiment" to load this file. \quit

CREATE OR REPLACE FUNCTION  timestamp_in_out_test() RETURNS TEXT
AS 'MODULE_PATHNAME', 'timestamp_in_out_test' LANGUAGE C IMMUTABLE PARALLEL SAFE STRICT;
