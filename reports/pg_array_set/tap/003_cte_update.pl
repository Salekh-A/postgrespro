use strict;
use warnings;
use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('node_cte');
$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_set');

# 1. Вызов функции внутри CTE (WITH)
my $result = $node->safe_psql('postgres', "
    WITH combined AS (
        SELECT array_union(ARRAY[1,2], ARRAY[2,3]) AS union_arr
    )
    SELECT union_arr FROM combined;
");
is($result, '{1,2,3}', 'array_union works inside CTE');

# 2. Использование функции в UPDATE таблицы
$node->safe_psql('postgres', 'CREATE TABLE test_update (id int, arr int[])');
$node->safe_psql('postgres', "INSERT INTO test_update VALUES (1, ARRAY[1,2])");
$node->safe_psql('postgres', "UPDATE test_update SET arr = array_union(arr, ARRAY[3]) WHERE id = 1");

$result = $node->safe_psql('postgres', "SELECT arr FROM test_update WHERE id = 1");
is($result, '{1,2,3}', 'array_union works in UPDATE');

$node->stop;
done_testing();
