#include <postgres.h>
#include <utils/builtins.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_palloc);

Datum
experiment_palloc(PG_FUNCTION_ARGS)
{
	char *buffcopy, *fmtstr;
	char *mybuff = (char*)palloc(128); /* or palloc0() */
	snprintf(mybuff, 128, "test data");
	elog(NOTICE, "mybuff after palloc() = %s", mybuff);

	mybuff = repalloc(mybuff, 256);
	elog(NOTICE, "mybuff after repalloc() = %s", mybuff);

	buffcopy = pstrdup(mybuff);
	pfree(mybuff);

	elog(NOTICE, "byffcopy = %s", buffcopy);

	fmtstr = psprintf("This is %s example", "psprintf()");
	elog(NOTICE, "fmtstr = %s", fmtstr);


	PG_RETURN_VOID();
}
