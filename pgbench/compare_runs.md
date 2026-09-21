1 прогон
```
S1024MBL0C1.report: TPS=414.25, latency=2.418 ms
S1024MBL0C10.report: TPS=1463.30, latency=6.957 ms
S1024MBL0C20.report: TPS=1560.98, latency=12.832 ms
S1024MBL0C5.report: TPS=1109.00, latency=4.525 ms
S1024MBL0C50.report: TPS=1845.93, latency=27.111 ms
S1024MBL1C1.report: TPS=396.51, latency=2.533 ms
S1024MBL1C10.report: TPS=1453.48, latency=6.880 ms
S1024MBL1C20.report: TPS=1662.12, latency=12.071 ms
S1024MBL1C5.report: TPS=986.41, latency=5.083 ms
S1024MBL1C50.report: TPS=1748.35, latency=28.610 ms
S512MBL0C1.report: TPS=372.41, latency=2.701 ms
S512MBL0C10.report: TPS=1715.78, latency=5.835 ms
S512MBL0C20.report: TPS=1868.11, latency=10.730 ms
S512MBL0C5.report: TPS=1245.51, latency=4.019 ms
S512MBL0C50.report: TPS=1912.92, latency=26.144 ms
S512MBL1C1.report: TPS=448.16, latency=2.232 ms
S512MBL1C10.report: TPS=1541.53, latency=6.490 ms
S512MBL1C20.report: TPS=1793.11, latency=11.168 ms
S512MBL1C5.report: TPS=1130.04, latency=4.433 ms
S512MBL1C50.report: TPS=1908.23, latency=26.230 ms
```

2 прогон
```
S1024MBL0C1.report: TPS=428.51, latency=2.348 ms
S1024MBL0C10.report: TPS=1464.52, latency=6.910 ms
S1024MBL0C20.report: TPS=1627.11, latency=12.324 ms
S1024MBL0C5.report: TPS=1106.92, latency=4.583 ms
S1024MBL0C50.report: TPS=1873.24, latency=26.716 ms
S1024MBL1C1.report: TPS=367.24, latency=2.851 ms
S1024MBL1C10.report: TPS=1497.58, latency=6.687 ms
S1024MBL1C20.report: TPS=1698.89, latency=11.809 ms
S1024MBL1C5.report: TPS=1070.59, latency=4.712 ms
S1024MBL1C50.report: TPS=1824.28, latency=27.482 ms
S512MBL0C1.report: TPS=400.50, latency=2.519 ms
S512MBL0C10.report: TPS=1742.98, latency=5.748 ms
S512MBL0C20.report: TPS=1910.31, latency=10.501 ms
S512MBL0C5.report: TPS=1289.04, latency=3.890 ms
S512MBL0C50.report: TPS=1933.90, latency=25.862 ms
S512MBL1C1.report: TPS=447.97, latency=2.233 ms
S512MBL1C10.report: TPS=1525.05, latency=6.567 ms
S512MBL1C20.report: TPS=1685.20, latency=12.116 ms
S512MBL1C5.report: TPS=1144.02, latency=4.385 ms
S512MBL1C50.report: TPS=1897.95, latency=26.371 ms
```

Выводы:
```
При росте числа клиентов TPS (кол-во транзакций в секунду), latency(среднее время выполнения одной транзакции) также растут
Расширение pg_stat_statements слегка снижает TPS при 10-50 клиентах
Разница между 512 МБ и 1024 МБ незначительна
```
