EXTENSION = experiment
MODULES = experiment
DATA = experiment--1.0.sql

PG_CPPFLAGS = -g -O0
SHLIB_LINK = 

TAP_TESTS = 1

ifndef PG_CONFIG
	PG_CONFIG := pg_config
endif
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
