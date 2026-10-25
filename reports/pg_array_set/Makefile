MODULES = pg_array_set
EXTENSION = pg_array_set
DATA = pg_array_set--1.0.sql
REGRESS = 01_basic_int 02_text 03_empty 04_duplicates 05_full_match 06_error_ndim 07_error_null_elem 08_null_arg 09_bool 10_text_edge
TAP_TESTS = 1

PG_CONFIG = $(HOME)/pg-install/bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
