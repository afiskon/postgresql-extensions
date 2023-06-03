EXTENSION = experiment
MODULES = experiment
DATA = experiment--1.0.sql experiment.conf
REGRESS_OPTS = --temp-config $(top_srcdir)/../../../share/postgresql/extension/experiment.conf
REGRESS = experiment

PG_CPPFLAGS = -g -O0
SHLIB_LINK = 

ifndef PG_CONFIG
	PG_CONFIG := pg_config
endif
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)