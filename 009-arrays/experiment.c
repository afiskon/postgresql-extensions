#include <postgres.h>
#include <utils/builtins.h>
#include <utils/array.h>
#include <catalog/pg_type_d.h>
#include <utils/typcache.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(experiment_sum);
PG_FUNCTION_INFO_V1(experiment_max);
PG_FUNCTION_INFO_V1(experiment_repeat);

Datum
experiment_sum(PG_FUNCTION_ARGS)
{
    ArrayType  *array = PG_GETARG_ARRAYTYPE_P(0);
    int ndim = ARR_NDIM(array);
    Oid elmtyp = ARR_ELEMTYPE(array);
    TypeCacheEntry *typentry;
    Datum *elmvalues;
    bool *elmnulls;
    int i, sum, elmcount;

    if (ndim > 1)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("multi-dimensional arrays are not supported")));

    /* Should never happen */
    if(elmtyp != INT4OID)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("only integer arrays are supported")));

    /* Empty array? */
    if (ndim < 1)
        PG_RETURN_INT32(0);

    typentry = (TypeCacheEntry *) fcinfo->flinfo->fn_extra;
    if (typentry == NULL)
    {
        typentry = lookup_type_cache(elmtyp, 0);
        fcinfo->flinfo->fn_extra = (void *) typentry;
    }

    deconstruct_array(
        array,
        elmtyp,
        typentry->typlen,
        typentry->typbyval,
        typentry->typalign,
        &elmvalues,
        &elmnulls,
        &elmcount
    );

    sum = 0;
    for (i = 0; i < elmcount; i++)
    {
        if (!elmnulls[i])
            sum += DatumGetInt32(elmvalues[i]);
    }

    PG_RETURN_INT32(sum);
}

Datum
experiment_max(PG_FUNCTION_ARGS)
{
    Oid collation = PG_GET_COLLATION();
    ArrayType  *array = PG_GETARG_ARRAYTYPE_P(0);
    int ndim = ARR_NDIM(array);
    Oid elmtyp = ARR_ELEMTYPE(array);
    TypeCacheEntry *typentry;
    Datum *elmvalues;
    bool *elmnulls;
    int i, elmcount;
    int32 cmp_result;
    bool resnull = true;
    Datum result = 0;

    if (ndim > 1)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("multi-dimensional arrays are not supported")));

    /* Empty array? */
    if (ndim < 1)
        PG_RETURN_NULL();

    typentry = (TypeCacheEntry *) fcinfo->flinfo->fn_extra;
    if (typentry == NULL || typentry->type_id != elmtyp)
    {
        typentry = lookup_type_cache(elmtyp, TYPECACHE_CMP_PROC_FINFO);
        fcinfo->flinfo->fn_extra = (void *) typentry;
    }

    deconstruct_array(
        array,
        elmtyp,
        typentry->typlen,
        typentry->typbyval,
        typentry->typalign,
        &elmvalues,
        &elmnulls,
        &elmcount
    );

    for (i = 0; i < elmcount; i++)
    {
        if (elmnulls[i])
            continue;

        if(resnull) {
            result = elmvalues[i];
            resnull = false;
            continue;
        }

        cmp_result = DatumGetInt32(FunctionCall2Coll(&typentry->cmp_proc_finfo, collation, result, elmvalues[i]));
        if(cmp_result < 0)
            result = elmvalues[i];
    }

    if(resnull)
        PG_RETURN_NULL();

    return result;
}

Datum
experiment_repeat(PG_FUNCTION_ARGS)
{
    Datum value = PG_GETARG_DATUM(0);
    int32 count = PG_GETARG_INT32(1);
    Oid elmtyp = get_fn_expr_argtype(fcinfo->flinfo, 0);
    TypeCacheEntry *typentry;
    ArrayType* result;
    Datum *elems;
    int dims[1];
    int lbs[1];
    int i;

    if (count < 0)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("count must not be negative")));

    if (count == 0) {
        result = construct_empty_array(elmtyp);
        PG_RETURN_ARRAYTYPE_P(result);
    }

    typentry = (TypeCacheEntry *) fcinfo->flinfo->fn_extra;
    if (typentry == NULL || typentry->type_id != elmtyp)
    {
        typentry = lookup_type_cache(elmtyp, TYPECACHE_CMP_PROC_FINFO);
        fcinfo->flinfo->fn_extra = (void *) typentry;
    }

    elems = (Datum *) palloc(sizeof(Datum) * count);
    for (i = 0; i < count; i++)
        elems[i] = value;

    dims[0] = count; /* construct a 1-dimensional array */
    lbs[0] = 1; /* array lower bounds */
    result = construct_md_array(elems, NULL, 1, dims, lbs, elmtyp,
        typentry->typlen, typentry->typbyval, typentry->typalign);
    PG_RETURN_ARRAYTYPE_P(result);
}
