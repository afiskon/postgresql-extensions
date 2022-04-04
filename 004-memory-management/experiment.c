#include <postgres.h>
#include <utils/builtins.h>
#include <utils/memutils.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(experiment_palloc);
PG_FUNCTION_INFO_V1(experiment_memctx);
PG_FUNCTION_INFO_V1(experiment_tryfinally);

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

static void
reset_callback(void* arg)
{
	elog(NOTICE, "reset_callback() called with arg = %s", (char*)arg);
}

Datum
experiment_memctx(PG_FUNCTION_ARGS)
{
	MemoryContextCallback* cb;
	Size buffsize, totalsize;

	MemoryContext myctx = AllocSetContextCreate(CurrentMemoryContext, "MemCtx", ALLOCSET_DEFAULT_SIZES);
	MemoryContext oldctx = MemoryContextSwitchTo(myctx);

	cb = (MemoryContextCallback*)palloc(sizeof(MemoryContextCallback));
	cb->func = reset_callback;
	cb->arg = pstrdup("memctx");
	MemoryContextRegisterResetCallback(myctx, cb);

	buffsize = GetMemoryChunkSpace(cb);
	totalsize = MemoryContextMemAllocated(myctx, true /* recursive */);
	elog(NOTICE, "Memory allocated for cb: %lld, sizeof(*cb) = %lld", (long long)buffsize, (long long)sizeof(*cb));
	elog(NOTICE, "Total memory allocated: %lld", (long long)totalsize);

	MemoryContextSwitchTo(oldctx);

	elog(NOTICE, "Calling MemoryContextDelete()...");
	MemoryContextDelete(myctx);
	elog(NOTICE, "Returning from experiment_memctx() ...");
	PG_RETURN_VOID();
}

Datum
experiment_tryfinally(PG_FUNCTION_ARGS)
{
	bool fail = BoolGetDatum(PG_GETARG_DATUM(0));

	PG_TRY();
	{
		MemoryContextCallback* cb;
		MemoryContext myctx = AllocSetContextCreate(CurrentMemoryContext, "TryCatch", ALLOCSET_DEFAULT_SIZES);
		/* MemoryContext oldctx = */ MemoryContextSwitchTo(myctx);

		cb = (MemoryContextCallback*)palloc(sizeof(MemoryContextCallback));
		cb->func = reset_callback;
		cb->arg = pstrdup("trycatch");
		MemoryContextRegisterResetCallback(myctx, cb);

		if(fail) {
			elog(ERROR, "oops...");
		}
	}
	PG_FINALLY();
	{
		elog(NOTICE, "cleaning up");
	}
	PG_END_TRY();

	PG_RETURN_VOID();
}