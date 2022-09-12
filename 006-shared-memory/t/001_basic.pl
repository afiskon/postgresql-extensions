
use strict;
use warnings;

use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('main');

$node->init;
$node->append_conf(
	'postgresql.conf',
	qq{shared_preload_libraries = 'experiment'});
$node->start;

$node->safe_psql("postgres",
	    "CREATE EXTENSION experiment;\n");

my $result =
  $node->safe_psql("postgres", "SELECT experiment_get_message();");
ok($result eq "hello", 'experiment_get_message() test succeeded');

$node->safe_psql("postgres",
	    "DROP EXTENSION experiment;\n");

$node->stop;

done_testing();