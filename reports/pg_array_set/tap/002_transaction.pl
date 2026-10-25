use strict;
use warnings;
use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('node_tx');
$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE TABLE test_arrays (id int, arr int[])');
$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_set');

# Начинаем транзакцию, вставляем данные, используя нашу функцию
$node->safe_psql('postgres', "
    BEGIN;
    INSERT INTO test_arrays VALUES (1, array_union(ARRAY[1,2], ARRAY[3]));
    ROLLBACK;
");

# Проверяем, что после ROLLBACK таблица пуста (функция не сломала транзакцию)
my $count = $node->safe_psql('postgres', "SELECT count(*) FROM test_arrays");
is($count, '0', 'ROLLBACK works correctly after using array_union');

# Проверяем COMMIT
$node->safe_psql('postgres', "
    BEGIN;
    INSERT INTO test_arrays VALUES (2, array_intersection(ARRAY[1,2,3], ARRAY[2,3]));
    COMMIT;
");

my $result = $node->safe_psql('postgres', "SELECT arr FROM test_arrays WHERE id = 2");
is($result, '{2,3}', 'COMMIT works correctly with array_intersection');

$node->stop;
done_testing();
