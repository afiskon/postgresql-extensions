#include <postgres.h>
#include <utils/builtins.h>
#include <datatype/timestamp.h>
#include <utils/timestamp.h>

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(timestamp_in_out_test);

Datum
timestamp_in_out_test(PG_FUNCTION_ARGS)
{
	static const char* tstamp_str = "Aug 05 00:00:00 1988";
	Timestamp tstamp;

	tstamp = DatumGetTimestamp(DirectFunctionCall3(timestamp_in,
		CStringGetDatum(tstamp_str),
		ObjectIdGetDatum(InvalidOid),
		Int32GetDatum(-1)));

	tstamp_str = DatumGetCString(DirectFunctionCall1(timestamp_out,
		TimestampGetDatum(tstamp)));

	PG_RETURN_TEXT_P(CStringGetTextDatum(tstamp_str));
}
