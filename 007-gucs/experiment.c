#include <postgres.h>
#include <miscadmin.h>
#include <utils/builtins.h>
#include <utils/guc.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_get_message);

static char *message = NULL;

void
_PG_init(void)
{
	elog(LOG, "_PG_init(): pid = %d, postmaster = %d",
		MyProcPid, !IsUnderPostmaster);

	DefineCustomStringVariable("experiment.message",
							   "Message to return via experiment_get_message()",
							   NULL,  /* long desc */
							   &message,
							   "",    /* initial value */
							   PGC_USERSET,
							   0,     /* flags */
							   NULL,  /* check hook */
							   NULL,  /* assign hook */
							   NULL); /* show hook */

	MarkGUCPrefixReserved("experiment");
}

Datum
experiment_get_message(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text(message));
}
