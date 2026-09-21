# Отладка PostgreSQL под GDB

# 1. Запускаем сервер  и получаем PID backend
```
abulg@HuaweiSx:~/demo_extension$ ~/pg-install/bin/pg_ctl -D ~/pgdata -l ~/pgdata/logfile start
waiting for server to start.... done
server started
abulg@HuaweiSx:~/demo_extension$ ~/pg-install/bin/psql -p 5432 -d postgres
psql (18.6)
Type "help" for help.

postgres=# SELECT pg_backend_pid();
 pg_backend_pid
----------------
          30102
(1 row)
```

# 2. Далее во втором терминале присоединяем GDB к backend
```
abulg@HuaweiSx:~$ sudo gdb -p 30102
GNU gdb (Ubuntu 15.1-1ubuntu1~24.04.1) 15.1
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word".
Attaching to process 29968
Reading symbols from /home/abulg/pg-install/bin/postgres...
Reading symbols from /lib/x86_64-linux-gnu/libz.so.1...
(No debugging symbols found in /lib/x86_64-linux-gnu/libz.so.1)
Reading symbols from /lib/x86_64-linux-gnu/libm.so.6...
Reading symbols from /usr/lib/debug/.build-id/d5/3fcbff9f855463606b25cfbe2a2e10856ea3a7.debug...
Reading symbols from /lib/x86_64-linux-gnu/libicui18n.so.74...
(No debugging symbols found in /lib/x86_64-linux-gnu/libicui18n.so.74)
Reading symbols from /lib/x86_64-linux-gnu/libicuuc.so.74...
(No debugging symbols found in /lib/x86_64-linux-gnu/libicuuc.so.74)
Reading symbols from /lib/x86_64-linux-gnu/libc.so.6...
Reading symbols from /usr/lib/debug/.build-id/a4/a7992a8e66555c8141ab2a08a8465ff6e0ea65.debug...
Reading symbols from /lib64/ld-linux-x86-64.so.2...
Reading symbols from /usr/lib/debug/.build-id/0d/77dbd4379bf0b5af3e05be7fb6af1e4cea7c96.debug...
Reading symbols from /lib/x86_64-linux-gnu/libstdc++.so.6...
(No debugging symbols found in /lib/x86_64-linux-gnu/libstdc++.so.6)
Reading symbols from /lib/x86_64-linux-gnu/libgcc_s.so.1...
(No debugging symbols found in /lib/x86_64-linux-gnu/libgcc_s.so.1)
Reading symbols from /lib/x86_64-linux-gnu/libicudata.so.74...
(No debugging symbols found in /lib/x86_64-linux-gnu/libicudata.so.74)
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
0x00007a4a4fb2a297 in epoll_wait (epfd=6,
    events=0x59a3436a8870, maxevents=1, timeout=-1)
    at ../sysdeps/unix/sysv/linux/epoll_wait.c:30

warning: 30     ../sysdeps/unix/sysv/linux/epoll_wait.c: No such file or directory
```
# 3. Точка остановки в расширении и ядре
```
(gdb) break exec_simple_query
Breakpoint 1 at 0x5c7a67f4393f: file postgres.c, line 1013.
(gdb) break hello_world
Function "hello_world" not defined.
Make breakpoint pending on future shared library load? (y or [n]) y
Breakpoint 2 (hello_world) pending.
(gdb) continue
Continuing.
```

# 4. Вызываем функцию hello world в основном терминале. GDB останавливается в ядре
```
Основной терминал:
postgres=# SELECT hello_world();

Второй терминал:
Breakpoint 1, exec_simple_query (
    query_string=0x5c7aa0bc7030 "SELECT hello_world();")
    at postgres.c:1013
1013    {
```
# 5. Стек вызовов в ядре
```
(gdb) bt
#0  exec_simple_query (
    query_string=0x5c7aa0bc7030 "SELECT hello_world();")
    at postgres.c:1013
#1  0x00005c7a67f4962a in PostgresMain (
    dbname=0x5c7aa0c01970 "postgres",
    username=0x5c7aa0c01958 "abulg") at postgres.c:4770
#2  0x00005c7a67f3f595 in BackendMain (
    startup_data=0x7ffc19525f50, startup_data_len=24)
    at backend_startup.c:124
#3  0x00005c7a67e3d157 in postmaster_child_launch (
    child_type=B_BACKEND, child_slot=2,
    startup_data=0x7ffc19525f50, startup_data_len=24,
    client_sock=0x7ffc19525fb0) at launch_backend.c:290
#4  0x00005c7a67e43b0c in BackendStartup (
    client_sock=0x7ffc19525fb0) at postmaster.c:3569
#5  0x00005c7a67e410fc in ServerLoop () at postmaster.c:1703
#6  0x00005c7a67e409eb in PostmasterMain (argc=3,
    argv=0x5c7aa0bc08f0) at postmaster.c:1401
#7  0x00005c7a67cda6c2 in main (argc=3, argv=0x5c7aa0bc08f0)
    at main.c:227
# 5. Продолжаем. GDB останавливается в расширении
(gdb) continue
Continuing.

Breakpoint 2, hello_world (fcinfo=0x5c7aa0caa1b8)
    at demo_extension.c:12
12          PG_RETURN_TEXT_P(cstring_to_text("Hello, world!"));
```

# 6. Стек вызовов в расширении
```
(gdb) bt
#0  hello_world (fcinfo=0x5c7aa0caa1b8) at demo_extension.c:12
#1  0x00005c7a67c37370 in ExecInterpExpr (
    state=0x5c7aa0caa060, econtext=0x5c7aa0ca9d08, isnull=0x0)
    at execExprInterp.c:934
#2  0x00005c7a67c3a189 in ExecInterpExprStillValid (
    state=0x5c7aa0caa060, econtext=0x5c7aa0ca9d08, isNull=0x0)
    at execExprInterp.c:2307
#3  0x00005c7a67c9ecf8 in ExecEvalExprNoReturn (
    state=0x5c7aa0caa060, econtext=0x5c7aa0ca9d08)
    at ../../../src/include/executor/executor.h:419
#4  0x00005c7a67c9edb6 in ExecEvalExprNoReturnSwitchContext (
    state=0x5c7aa0caa060, econtext=0x5c7aa0ca9d08)
    at ../../../src/include/executor/executor.h:460
#5  0x00005c7a67c9ee17 in ExecProject (projInfo=0x5c7aa0caa058)
    at ../../../src/include/executor/executor.h:492
#6  0x00005c7a67c9f03a in ExecResult (pstate=0x5c7aa0ca9bf8)
    at nodeResult.c:135
#7  0x00005c7a67c5441f in ExecProcNodeFirst (
    node=0x5c7aa0ca9bf8) at execProcnode.c:469
#8  0x00005c7a67c46948 in ExecProcNode (node=0x5c7aa0ca9bf8)
    at ../../../src/include/executor/executor.h:315
#9  0x00005c7a67c49920 in ExecutePlan (
    queryDesc=0x5c7aa0bf1f70, operation=CMD_SELECT,
    sendTuples=true, numberTuples=0,
    direction=ForwardScanDirection, dest=0x5c7aa0bf30c0)
    at execMain.c:1711
#10 0x00005c7a67c4700b in standard_ExecutorRun (
    queryDesc=0x5c7aa0bf1f70, direction=ForwardScanDirection,
    count=0) at execMain.c:366
--Type <RET> for more, q to quit, c to continue without paging--c
#11 0x00005c7a67c46e69 in ExecutorRun (
    queryDesc=0x5c7aa0bf1f70, direction=ForwardScanDirection,
    count=0) at execMain.c:303
#12 0x00005c7a67f4bad1 in PortalRunSelect (
    portal=0x5c7aa0c481d0, forward=true, count=0,
    dest=0x5c7aa0bf30c0) at pquery.c:921
#13 0x00005c7a67f4b705 in PortalRun (portal=0x5c7aa0c481d0,
    count=9223372036854775807, isTopLevel=true,
    dest=0x5c7aa0bf30c0, altdest=0x5c7aa0bf30c0,
    qc=0x7ffc19525ca0) at pquery.c:765
#14 0x00005c7a67f43e61 in exec_simple_query (
    query_string=0x5c7aa0bc7030 "SELECT hello_world();")
    at postgres.c:1274
#15 0x00005c7a67f4962a in PostgresMain (
    dbname=0x5c7aa0c01970 "postgres",
    username=0x5c7aa0c01958 "abulg") at postgres.c:4770
#16 0x00005c7a67f3f595 in BackendMain (
    startup_data=0x7ffc19525f50, startup_data_len=24)
    at backend_startup.c:124
#17 0x00005c7a67e3d157 in postmaster_child_launch (
    child_type=B_BACKEND, child_slot=2,
    startup_data=0x7ffc19525f50, startup_data_len=24,
    client_sock=0x7ffc19525fb0) at launch_backend.c:290
#18 0x00005c7a67e43b0c in BackendStartup (
    client_sock=0x7ffc19525fb0) at postmaster.c:3569
#19 0x00005c7a67e410fc in ServerLoop () at postmaster.c:1703
#20 0x00005c7a67e409eb in PostmasterMain (argc=3,
    argv=0x5c7aa0bc08f0) at postmaster.c:1401
#21 0x00005c7a67cda6c2 in main (argc=3, argv=0x5c7aa0bc08f0)
    at main.c:227
```
# 7. Аргументы
```
(gdb) print fcinfo
$1 = (FunctionCallInfo) 0x5c7aa0ca1078

(gdb) info args
fcinfo = 0x5c7aa0ca1078
```
# 8. Наблюдение 
```
(gdb) watch fcinfo
Hardware watchpoint 3: fcinfo

(gdb) continue
Continuing.

Watchpoint 3 deleted because the program has left the block
in which its expression is valid.
```
# 9. Результат в psql
```
postgres=# SELECT hello_world();
  hello_world
---------------
 Hello, world!
(1 row)

postgres=#
```
# 10. Отсоединение 
```
(gdb) detach
Detaching from program: /home/abulg/pg-install/bin/postgres, process 30194
[Inferior 1 (process 30194) detached]
(gdb) quit
```
