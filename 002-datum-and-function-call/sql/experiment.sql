CREATE EXTENSION experiment;

SELECT timestamp_in_out_test();
SELECT timestamptz_zone_test(null);
SELECT timestamptz_zone_test('-infinity');
SELECT timestamptz_zone_test('2010-01-01 00:00:00 MSK');
SELECT timestamptz_zone_test('2010-01-01 00:00:00 HST', 'Pacific/Honolulu');

\set ON_ERROR_STOP 0
SELECT timestamptz_zone_test('2010-01-01 00:00:00 GMT', 'Europe/Ololondon');
\set ON_ERROR_STOP 1

DROP EXTENSION experiment;