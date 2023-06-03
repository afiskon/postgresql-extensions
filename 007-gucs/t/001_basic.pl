
use strict;
use warnings;

use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $result;
my $cmdret;
my $node = PostgreSQL::Test::Cluster->new('main');

$node->init;
$node->append_conf('postgresql.conf', qq{shared_preload_libraries = 'experiment'});
$node->append_conf('postgresql.conf', qq{experiment.message = 'ololotrololo'});
$node->start;

$node->safe_psql("postgres", "CREATE EXTENSION experiment;");

$result = $node->safe_psql("postgres", "SELECT experiment_get_message();");
ok($result eq "ololotrololo", 'changing the config value has an effect');

$node->safe_psql("postgres", "DROP EXTENSION experiment;");

$node->stop;

done_testing();
