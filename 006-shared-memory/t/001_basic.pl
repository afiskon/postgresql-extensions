
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
$node->start;

$node->safe_psql("postgres", "CREATE EXTENSION experiment;");

$result = $node->safe_psql("postgres", "SELECT experiment_get_message();");
ok($result eq "", 'the initial message is empty');

$node->safe_psql("postgres", "SELECT experiment_set_message('hello');");
$result = $node->safe_psql("postgres", "SELECT experiment_get_message();");
ok($result eq "hello", 'the message was updated');

# Make sure the lock will be released if we throw an error
$cmdret = $node->psql("postgres", "SELECT experiment_lock_and_throw_error();");
ok($cmdret != 0, 'an error was thrown');

$result = $node->safe_psql("postgres", "SELECT experiment_get_message();");
ok($result eq "hello", 'the lock was released');

$node->safe_psql("postgres", "DROP EXTENSION experiment;");

$node->stop;

done_testing();
