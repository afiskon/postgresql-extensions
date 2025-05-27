-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION experiment" to load this file. \quit

CREATE OR REPLACE FUNCTION experiment_get_message() RETURNS TEXT
AS 'MODULE_PATHNAME', 'experiment_get_message' LANGUAGE C;

CREATE OR REPLACE FUNCTION experiment_set_message(TEXT) RETURNS VOID
AS 'MODULE_PATHNAME', 'experiment_set_message' LANGUAGE C;

CREATE OR REPLACE FUNCTION experiment_lock_and_throw_error() RETURNS VOID
AS 'MODULE_PATHNAME', 'experiment_lock_and_throw_error' LANGUAGE C;
