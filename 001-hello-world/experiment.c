#include <postgres.h>
#include <port.h>
#include <catalog/pg_type.h>
#include <executor/spi.h>
#include <utils/builtins.h>
#include <utils/jsonb.h>
#include <sys/time.h>
#include <limits.h>
#include <string.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_hello);

Datum
experiment_hello(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text("hello"));
}
