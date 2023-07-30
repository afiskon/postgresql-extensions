
use strict;
use warnings;

use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $result;
my $cmdret;
my $node = PostgreSQL::Test::Cluster->new('main');

$node->init;
# $node->append_conf('postgresql.conf', qq{shared_preload_libraries = 'experiment'});
$node->append_conf('postgresql.conf', qq{experiment.message = 'ololotrololo'});
$node->start;

$node->safe_psql("postgres", "CREATE EXTENSION experiment;");
$result = $node->safe_psql("postgres", qq{
SELECT experiment_get_message();
});
ok($result eq "ololotrololo", 'changing postgresql.conf has an effect');

$result = $node->safe_psql("postgres", qq{
SET experiment.message TO 'abc';
SELECT experiment_get_message();
});
ok($result eq "abc", 'SET has an effect');

$result = $node->safe_psql("postgres", qq{
SET experiment.message TO 'abc';
RESET experiment.message;
SELECT experiment_get_message();
});
ok($result eq "ololotrololo", 'RESET has an effect');

$node->safe_psql("postgres", "DROP EXTENSION experiment;");
$node->stop;

done_testing();
