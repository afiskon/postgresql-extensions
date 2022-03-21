#include <postgres.h>
#include <utils/builtins.h>
#include <access/xact.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_elog);
PG_FUNCTION_INFO_V1(experiment_ereport);
PG_FUNCTION_INFO_V1(experiment_trycatch);

Datum
experiment_elog(PG_FUNCTION_ARGS)
{
    /* int64 now = (int64)GetCurrentTransactionStartTimestamp(); */
    int64 now = 123456789;

    /* NB: %m corresponds to strerror(errno) */
    elog(NOTICE, "Transaction start timestamp: %lld",
        (long long)now);
    PG_RETURN_VOID();
}

Datum
experiment_ereport(PG_FUNCTION_ARGS)
{
    const char* arg = TextDatumGetCString(PG_GETARG_DATUM(0));
    ereport(ERROR,
        (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
         errmsg("Invalid argument \"%s\"", arg),
         errhint("This is a hint message.")));
    PG_RETURN_VOID();
}

Datum
experiment_trycatch(PG_FUNCTION_ARGS)
{
    PG_TRY();
    {
        elog(ERROR, "oops...");
    }
    PG_FINALLY();
    {
        elog(NOTICE, "cleaning up");
    }
    PG_END_TRY();

    PG_RETURN_VOID();
}