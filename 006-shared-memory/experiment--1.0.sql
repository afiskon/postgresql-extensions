-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION experiment" to load this file. \quit

CREATE OR REPLACE FUNCTION  experiment_get_message() RETURNS TEXT
AS 'MODULE_PATHNAME', 'experiment_get_message' LANGUAGE C;
