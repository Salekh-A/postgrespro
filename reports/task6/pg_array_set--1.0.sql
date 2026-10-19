\echo Use "CREATE EXTENSION pg_array_set" to load this file. \quit

CREATE FUNCTION array_intersection(anycompatiblearray, anycompatiblearray)
RETURNS anycompatiblearray
AS 'MODULE_PATHNAME', 'array_intersection'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_union(anycompatiblearray, anycompatiblearray)
RETURNS anycompatiblearray
AS 'MODULE_PATHNAME', 'array_union'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_except(anycompatiblearray, anycompatiblearray)
RETURNS anycompatiblearray
AS 'MODULE_PATHNAME', 'array_except'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_symmetric_difference(anycompatiblearray, anycompatiblearray)
RETURNS anycompatiblearray
AS 'MODULE_PATHNAME', 'array_symmetric_difference'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION array_unique(anyarray)
RETURNS anyarray
AS 'MODULE_PATHNAME', 'array_unique'
LANGUAGE C IMMUTABLE STRICT;
