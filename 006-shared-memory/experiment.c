#include <postgres.h>
#include <miscadmin.h>
#include <utils/builtins.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_hello);

void
_PG_init(void)
{
	if(!process_shared_preload_libraries_in_progress)
		elog(ERROR, "Please use shared_preload_libraries");


	elog(NOTICE, "_PG_init() called...");
}

Datum
experiment_hello(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text("hello"));
}
