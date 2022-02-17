#include <postgres.h>
#include <utils/builtins.h>
#include <datatype/timestamp.h>
#include <utils/timestamp.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(timestamp_in_out_test);
PG_FUNCTION_INFO_V1(timestamptz_zone_test);

Datum
timestamp_in_out_test(PG_FUNCTION_ARGS)
{
    const char* tstamp_str = "Jan 01 00:00:00 2010";
    Timestamp tstamp;

    tstamp = DatumGetTimestamp(DirectFunctionCall3(timestamp_in,
        CStringGetDatum(tstamp_str),
        ObjectIdGetDatum(InvalidOid),
        Int32GetDatum(-1)));

    tstamp_str = DatumGetCString(DirectFunctionCall1(timestamp_out,
        TimestampGetDatum(tstamp)));

    PG_RETURN_TEXT_P(CStringGetTextDatum(tstamp_str));
}

Datum
timestamptz_zone_test(PG_FUNCTION_ARGS)
{
    Timestamp result;
    TimestampTz timestamptz = TimestampTzGetDatum(PG_GETARG_DATUM(0));
    const char* tzname = "Europe/Moscow";

    if(TIMESTAMP_NOT_FINITE(timestamptz)) {
        // Timestamp and TimestampTz internally are the same in PostgreSQL
        // The TZ-version of TIMESTAMP_NOT_FINITE() is not even defined!
        result = (Timestamp)timestamptz;
        PG_RETURN_TIMESTAMP(result);
    }

    if(PG_NARGS() > 1) {
        tzname = TextDatumGetCString(PG_GETARG_DATUM(1));
    }

    // The code is equal to 'timestamptz AT TIME ZONE tzname'
    result = DatumGetTimestamp(DirectFunctionCall2(timestamptz_zone,
            CStringGetTextDatum(tzname),
            TimestampTzGetDatum(timestamptz)));

    PG_RETURN_TIMESTAMP(result);
}
