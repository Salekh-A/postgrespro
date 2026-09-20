#1 инициализирование тестового бд
abulg@HuaweiSx:~/postgrespro/reports$ ~/pg-install/bin/createdb -p 5432 pgbench_test
abulg@HuaweiSx:~/postgrespro/reports$ ~/pg-install/bin/pgbench -i -s 50 -p 5432 pgbench_test
dropping old tables...
NOTICE:  table "pgbench_accounts" does not exist, skipping
NOTICE:  table "pgbench_branches" does not exist, skipping
NOTICE:  table "pgbench_history" does not exist, skipping
NOTICE:  table "pgbench_tellers" does not exist, skipping
creating tables...
generating data (client-side)...
vacuuming...
creating primary keys...
done in 19.96 s (drop tables 0.01 s, create tables 0.03 s, client-side generate 14.85 s, vacuum 0.53 s, primary keys 4.55 s).

#2 прогон без расширений
abulg@HuaweiSx:~/postgrespro/reports$ ~/pg-install/bin/pgbench -c 10 -j 2 -t 1000 -p 5432 pgbench_test
pgbench (18.6)
starting vacuum...end.
transaction type: <builtin: TPC-B (sort of)>
scaling factor: 50
query mode: simple
number of clients: 10
number of threads: 2
maximum number of tries: 1
number of transactions per client: 1000
number of transactions actually processed: 10000/10000
number of failed transactions: 0 (0.000%)
latency average = 6.026 ms
initial connection time = 42.139 ms
tps = 1659.371241 (without initial connection time)

#3 прогон с расширением
abulg@HuaweiSx:~/postgresql-18.6/contrib/pg_stat_statements$ ~/pg-install/bin/pgbench -c 10 -j 2 -t 1000 -p 5432 pgbench_test
pgbench (18.6)
starting vacuum...end.
transaction type: <builtin: TPC-B (sort of)>
scaling factor: 50
query mode: simple
number of clients: 10
number of threads: 2
maximum number of tries: 1
number of transactions per client: 1000
number of transactions actually processed: 10000/10000
number of failed transactions: 0 (0.000%)
latency average = 5.760 ms
initial connection time = 27.160 ms
tps = 1736.149692 (without initial connection time)
