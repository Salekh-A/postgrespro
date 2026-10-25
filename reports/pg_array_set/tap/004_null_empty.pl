use strict;
use warnings;
use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('node_null');
$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_set');

# 1. Пустые массивы
my $result = $node->safe_psql('postgres', "SELECT array_union(ARRAY[]::int[], ARRAY[]::int[])");
is($result, '{}', 'array_union of two empty arrays returns empty array');

$result = $node->safe_psql('postgres', "SELECT array_intersection(ARRAY[]::int[], ARRAY[1,2])");
is($result, '{}', 'array_intersection with empty array returns empty array');

# 2. NULL как аргумент (должен вернуть NULL благодаря STRICT)
$result = $node->safe_psql('postgres', "SELECT array_unique(NULL::int[]) IS NULL");
is($result, 't', 'array_unique(NULL) returns NULL (STRICT)');

# 3. NULL внутри массива (должна быть ошибка)
my ($ret, $stdout, $stderr) = $node->psql('postgres', "SELECT array_union(ARRAY[1,NULL], ARRAY[2,3])");
isnt($ret, 0, 'NULL element inside array raises error');
like($stderr, qr/NULL elements are not allowed/, 'correct error message for NULL element');

# 4. Многомерный массив (должна быть ошибка)
($ret, $stdout, $stderr) = $node->psql('postgres', "SELECT array_unique(ARRAY[[1,2],[3,4]])");
isnt($ret, 0, 'multidimensional array raises error');
like($stderr, qr/only 1-D arrays are supported/, 'correct error message for multidimensional array');

$node->stop;
done_testing();
