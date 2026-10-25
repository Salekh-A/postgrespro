use strict;
use warnings;
use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('node_basic');
$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_set');

my $result = $node->safe_psql('postgres', "SELECT array_intersection(ARRAY[1,2,3,2], ARRAY[2,3,4])");
is($result, '{2,3}', 'array_intersection works');

$result = $node->safe_psql('postgres', "SELECT array_union(ARRAY[1,2], ARRAY[2,3])");
is($result, '{1,2,3}', 'array_union works');

$result = $node->safe_psql('postgres', "SELECT array_unique(ARRAY['a','b','a','c'])");
is($result, '{a,b,c}', 'array_unique works');

my ($ret, $stdout, $stderr) = $node->psql('postgres', "SELECT array_union(ARRAY[1,NULL], ARRAY[2,3])");
isnt($ret, 0, 'NULL element raises error');
like($stderr, qr/NULL elements are not allowed/, 'correct error message');

$node->stop;
done_testing();
