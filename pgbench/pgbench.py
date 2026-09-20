import re
import subprocess
import argparse
from pgbench_config import *


def set_conf(param, value):
    text = postgres_conf.read_text()
    text = re.sub(
        rf"^(#?){re.escape(param)}\s*=.*$",
        f"{param} = '{value}'",
        text,
        flags=re.MULTILINE,
    )
    postgres_conf.write_text(text)


def startup(lib):
    set_conf("shared_preload_libraries", lib)
    subprocess.run(f"{pg_bin}/pg_ctl -D {pgdata} -l {pgdata}/logfile start", shell=True)

    if lib:
        subprocess.run(
            f"{pg_bin}/psql -p {port} -U {user} -d {db} "
            f"-c 'CREATE EXTENSION IF NOT EXISTS {lib};'",
            shell=True,
        )


def shutdown(lib):
    if lib:
        subprocess.run(
            f"{pg_bin}/psql -p {port} -U {user} -d {db} "
            f"-c 'DROP EXTENSION IF EXISTS {lib};'",
            shell=True,
        )
    subprocess.run(f"{pg_bin}/pg_ctl -D {pgdata} stop -m fast", shell=True)


def bench(clients):
    out = subprocess.run(
        f"{pg_bin}/pgbench -c {clients} -j 2 -t {transactions} -p {port} {db}",
        shell=True,
        capture_output=True,
        text=True,
    ).stdout

    tps = re.search(r"tps = ([\d.]+)", out)
    lat = re.search(r"latency average = ([\d.]+)", out)

    if not tps or not lat:
        return None

    return float(tps.group(1)), float(lat.group(1))


def init_db():
    startup("")

    subprocess.run(
        f"{pg_bin}/psql -p {port} -U {user} -d postgres "
        f"-c 'DROP DATABASE IF EXISTS {db};'",
        shell=True,
    )
    subprocess.run(f"{pg_bin}/createdb -p {port} -U {user} {db}", shell=True)
    subprocess.run(f"{pg_bin}/pgbench -i -s {scale_factor} -p {port} {db}", shell=True)

    # останавливаем — дальше скрипт сам запустит
    shutdown("")


def run():
    reports_path.mkdir(parents=True, exist_ok=True)
    init_db()

    for sb in shared_buffers:
        set_conf("shared_buffers", sb)

        for lib in libs:
            startup(lib)

            for clients in client_nums:
                for i in iters:
                    res = bench(clients)
                    if res is None:
                        continue

                    tps, lat = res
                    path = reports_path / f"S{sb}L{int(bool(lib))}C{clients}.report"

                    with open(path, "a") as f:
                        f.write(f"iter={i} tps={tps} latency={lat}\n")

            shutdown(lib)


def clear():
    subprocess.run(f"rm -rf {reports_path}", shell=True)


def summary():
    print("\n=== СВОДКА ===\n")
    for f in sorted(reports_path.glob("*.report")):
        text = f.read_text()
        tps = [float(x) for x in re.findall(r"tps=([\d.]+)", text)]
        lat = [float(x) for x in re.findall(r"latency=([\d.]+)", text)]
        if not tps:
            continue
        mean_tps = sum(tps) / len(tps)
        mean_lat = sum(lat) / len(lat)
        print(f"{f.name}: TPS={mean_tps:.2f}, latency={mean_lat:.3f} ms")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--run", action="store_true")
    parser.add_argument("--summary", action="store_true")
    parser.add_argument("--clear", action="store_true")
    args = parser.parse_args()

    if args.run:
        run()
    if args.summary:
        summary()
    if args.clear:
        clear()


if __name__ == "__main__":
    main()
