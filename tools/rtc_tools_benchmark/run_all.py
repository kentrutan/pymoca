#!/usr/bin/env python3
"""Run every RTC-Tools example under one pymoca version and collect results.

For each ``examples/<name>/src/<script>.py`` in an RTC-Tools checkout this:

1. cleans both caches that would otherwise defeat a cold-compile comparison
   (RTC-Tools' ``<model>.pymoca_cache`` files and pymoca's own parse cache,
   the latter relocated to a throwaway ``XDG_CACHE_HOME`` that is wiped each
   run);
2. runs the script as a subprocess with the ``sitecustomize`` shim on
   ``PYTHONPATH`` so ``transfer_model`` is timed and fingerprinted;
3. captures stdout/stderr and copies the produced ``output/`` folder into a
   version-labelled results directory.

The compile-time and fingerprint data are written by the shim to ``BENCH_JSON``;
this script additionally writes ``run_index.json`` describing each script run
(exit status, wall time, output files).

Usage:
    run_all.py --python <venv python> --examples <dir> --label <name> \
               --out <results dir> [--timeout 900] [--only basic,mixed_integer]
"""

import argparse
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent


def discover(examples_dir: Path):
    """Yield (example_name, script_path) for every runnable example script."""
    runs = []
    for ex in sorted(p for p in examples_dir.iterdir() if p.is_dir()):
        src = ex / "src"
        if not src.is_dir():
            continue
        for script in sorted(src.glob("*.py")):
            if script.name.startswith("_"):
                continue
            runs.append((ex.name, script))
    return runs


def clean_caches(example_dir: Path, xdg_cache: Path):
    # RTC-Tools compiled-model cache written next to the model.
    for cache in example_dir.rglob("*.pymoca_cache"):
        try:
            cache.unlink()
        except OSError:
            pass
    # pymoca's own parse cache (PR branch keeps a sqlite DB under XDG cache).
    if xdg_cache.exists():
        shutil.rmtree(xdg_cache, ignore_errors=True)
    xdg_cache.mkdir(parents=True, exist_ok=True)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--python", required=True, help="python interpreter of the venv")
    ap.add_argument("--examples", required=True, help="path to rtc-tools/examples")
    ap.add_argument("--label", required=True, help="pymoca version label")
    ap.add_argument("--out", required=True, help="results output directory")
    ap.add_argument("--timeout", type=int, default=900)
    ap.add_argument("--only", default="", help="comma-separated example names to include")
    ap.add_argument(
        "--modelicapath",
        default="",
        help="value to export as MODELICAPATH for each run (e.g. an MSL root); "
        "falls back to the ambient MODELICAPATH when omitted",
    )
    args = ap.parse_args()

    examples_dir = Path(args.examples).resolve()
    out_dir = Path(args.out).resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    bench_json = out_dir / "compile.jsonl"
    if bench_json.exists():
        bench_json.unlink()
    xdg_cache = out_dir / "xdg_cache"

    only = {s for s in args.only.split(",") if s}
    runs = discover(examples_dir)
    if only:
        runs = [(n, s) for (n, s) in runs if n in only]

    index = []
    for name, script in runs:
        example_dir = script.parents[1]
        key = f"{name}__{script.stem}"
        print(f"[{args.label}] running {key} ...", flush=True)

        clean_caches(example_dir, xdg_cache)

        env = dict(os.environ)
        env["PYTHONPATH"] = str(HERE) + os.pathsep + env.get("PYTHONPATH", "")
        env["BENCH_JSON"] = str(bench_json)
        env["BENCH_EXAMPLE"] = key
        env["BENCH_PYMOCA_VER"] = args.label
        env["XDG_CACHE_HOME"] = str(xdg_cache)
        env["MPLBACKEND"] = "Agg"  # never pop up plot windows
        if args.modelicapath:
            # Put an MSL (or any shared library) on pymoca's search path so
            # models that `extend` MSL classes (e.g. Modelica.Icons.Package)
            # resolve. Honored by the patched CasADi backend; ignored by
            # pymoca versions that don't read MODELICAPATH.
            env["MODELICAPATH"] = args.modelicapath

        log_path = out_dir / f"{key}.log"
        start = time.perf_counter()
        try:
            with open(log_path, "w") as log:
                proc = subprocess.run(
                    [args.python, str(script)],
                    cwd=str(example_dir),
                    env=env,
                    stdout=log,
                    stderr=subprocess.STDOUT,
                    timeout=args.timeout,
                )
            rc = proc.returncode
            timed_out = False
        except subprocess.TimeoutExpired:
            rc = None
            timed_out = True
        elapsed = time.perf_counter() - start

        # Snapshot the example's output folder for later numeric comparison.
        out_src = example_dir / "output"
        out_snap = out_dir / "outputs" / key
        if out_snap.exists():
            shutil.rmtree(out_snap, ignore_errors=True)
        if out_src.is_dir():
            shutil.copytree(out_src, out_snap)

        index.append(
            {
                "key": key,
                "example": name,
                "script": str(script.relative_to(examples_dir.parent)),
                "returncode": rc,
                "timed_out": timed_out,
                "wall_seconds": elapsed,
                "log": log_path.name,
                "output_dir": str(out_snap.relative_to(out_dir)) if out_snap.exists() else None,
            }
        )
        status = "TIMEOUT" if timed_out else ("ok" if rc == 0 else f"FAIL rc={rc}")
        print(f"    -> {status} ({elapsed:.1f}s)", flush=True)

    (out_dir / "run_index.json").write_text(json.dumps(index, indent=2))
    n_ok = sum(1 for r in index if r["returncode"] == 0)
    print(f"[{args.label}] {n_ok}/{len(index)} scripts exited 0", flush=True)


if __name__ == "__main__":
    sys.exit(main())
