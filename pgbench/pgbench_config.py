from pathlib import Path

shared_buffers = ["512MB", "1024MB"]
libs = ["", "pg_stat_statements"]
client_nums = [1, 5, 10, 20, 50]
iters = [1, 2, 3]
transactions = 1000

db = "pgbench_test"
scale_factor = 50

host = "localhost"
port = "5432"
user = "abulg"

pgdata = Path.home() / "pgdata"
postgres_conf = pgdata / "postgresql.conf"

reports_path = Path(__file__).parent / "reports"
pg_bin = Path.home() / "pg-install/bin"
