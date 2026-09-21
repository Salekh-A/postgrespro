#1 инициализирование тестового бд
```
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
```
#2 3 прогона без расширений
```
Прогон 1
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

Прогон 2
scaling factor: 50
query mode: simple
number of clients: 10
number of threads: 2
maximum number of tries: 1
number of transactions per client: 1000
number of transactions actually processed: 10000/10000
number of failed transactions: 0 (0.000%)
latency average = 6.568 ms
initial connection time = 25.011 ms
tps = 1522.516110 (without initial connection time)

Прогон 3
scaling factor: 50
query mode: simple
number of clients: 10
number of threads: 2
maximum number of tries: 1
number of transactions per client: 1000
number of transactions actually processed: 10000/10000
number of failed transactions: 0 (0.000%)
latency average = 7.552 ms
initial connection time = 25.009 ms
tps = 1324.230397 (without initial connection time)
```
#3 3 прогона с расширениями
```
Прогон 1
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

Прогон 2
scaling factor: 50
query mode: simple
number of clients: 10
number of threads: 2
maximum number of tries: 1
number of transactions per client: 1000
number of transactions actually processed: 10000/10000
number of failed transactions: 0 (0.000%)
latency average = 7.163 ms
initial connection time = 31.657 ms
tps = 1396.151007 (without initial connection time)

Прогон 3
scaling factor: 50
query mode: simple
number of clients: 10
number of threads: 2
maximum number of tries: 1
number of transactions per client: 1000
number of transactions actually processed: 10000/10000
number of failed transactions: 0 (0.000%)
latency average = 7.061 ms
initial connection time = 54.318 ms
tps = 1416.218764 (without initial connection time)
```

Средние значения:
```
Без pg_stat_statements:
- средний TPS 1502.04
- средний Latency 6.715 ms

С pg_stat_statements:
- средний TPS 1516.17
- средний Latency 6.661 ms
```
Вывод: Если сравнивать средние значения особой разницы нет без расширения, либо с расширением (при 10 клиентах)


#4. Сравнение с Postgres 17.11 с Postgres 18
Результаты 3-х прогонов Postgres 17.11
```
abulg@HuaweiSx:~/postgresql-rel$ ~/pg-install-rel/bin/pgbench -c 10 -j 2 -t 1000 -p 5433 pgbench_test
pgbench (17.11)
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
latency average = 7.016 ms
initial connection time = 22.632 ms
tps = 1425.283909 (without initial connection time)
abulg@HuaweiSx:~/postgresql-rel$ ~/pg-install-rel/bin/pgbench -c 10 -j 2 -t 1000 -p 5433 pgbench_test
pgbench (17.11)
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
latency average = 6.463 ms
initial connection time = 27.069 ms
tps = 1547.228851 (without initial connection time)
abulg@HuaweiSx:~/postgresql-rel$ ~/pg-install-rel/bin/pgbench -c 10 -j 2 -t 1000 -p 5433 pgbench_test
pgbench (17.11)
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
latency average = 8.643 ms
initial connection time = 17.438 ms
tps = 1156.997771 (without initial connection time)
```
Средний TPS Postgres 17.11 - 1376.5
Средний Latency Postgres 17.11 - 7.374

Cредний TPS Postgres 18.6 - 1502.04
Cредний Latency Postgres 18.6 - 6.715 

Вывод: Postgres 18.6 лучше по средним показателям на 9% (Для более точного результата можно провести больше тестов). 

