#include "postgres_ext.h"
#include <postgres.h>
#include <utils/builtins.h>
#include <utils/typcache.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_max);

Datum
experiment_max(PG_FUNCTION_ARGS)
{
    Datum a = PG_GETARG_DATUM(0);
    Datum b = PG_GETARG_DATUM(1);
    Oid collation = PG_GET_COLLATION();
    Oid elmtyp = get_fn_expr_argtype(fcinfo->flinfo, 0);
    TypeCacheEntry *typentry;
    int cmp_result;

    typentry = (TypeCacheEntry *) fcinfo->flinfo->fn_extra;
    if (typentry == NULL || typentry->type_id != elmtyp)
    {
        typentry = lookup_type_cache(elmtyp, TYPECACHE_CMP_PROC_FINFO);
        fcinfo->flinfo->fn_extra = (void *) typentry;
    }

    cmp_result = DatumGetInt32(FunctionCall2Coll(&typentry->cmp_proc_finfo, collation, a, b));
    return cmp_result <= 0 ? b : a;
}
