\echo Use "CREATE EXTENSION demo_extension" to load this file. \quit

CREATE FUNCTION hello_world()
RETURNS text
AS 'MODULE_PATHNAME', 'hello_world'
LANGUAGE C STRICT;
