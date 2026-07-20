#!/usr/bin/env python3
"""Compare two pymoca versions' benchmark runs and emit a markdown report.

Reads the per-version results produced by ``run_all.py`` (``compile.jsonl``,
``run_index.json`` and the ``outputs/`` snapshots) for a baseline label and a
candidate label, then writes ``RESULTS.md`` containing:

* environment / version header;
* per-example compile-time table (baseline, candidate, ratio);
* model fingerprint comparison (variable & equation counts, name-set diffs);
* numerical results comparison of the exported timeseries CSVs;
* a list of any script that failed under either version.
"""

import argparse
import csv
import json
import math
from pathlib import Path


def load_jsonl(path: Path):
    records = {}
    if not path.exists():
        return records
    for line in path.read_text().splitlines():
        line = line.strip()
        if not line:
            continue
        rec = json.loads(line)
        records[rec["example"]] = rec  # last call wins (cache retry would overwrite)
    return records


def load_index(path: Path):
    if not path.exists():
        return {}
    return {r["key"]: r for r in json.loads(path.read_text())}


def read_timeseries_csv(path: Path):
    """Return (fieldnames, {column: [floats]}) for a timeseries_export.csv."""
    with open(path, newline="") as handle:
        reader = csv.reader(handle)
        rows = list(reader)
    if not rows:
        return [], {}
    header = rows[0]
    cols = {name: [] for name in header}
    for row in rows[1:]:
        for name, value in zip(header, row):
            try:
                cols[name].append(float(value))
            except (ValueError, TypeError):
                cols[name].append(math.nan)
    return header, cols


def compare_csv(base_csv: Path, pr_csv: Path):
    """Return (max_abs, max_rel, note) comparing two timeseries CSVs."""
    bh, bcols = read_timeseries_csv(base_csv)
    ph, pcols = read_timeseries_csv(pr_csv)
    shared = [c for c in bh if c in pcols]
    only_base = [c for c in bh if c not in pcols]
    only_pr = [c for c in ph if c not in bcols]
    max_abs = 0.0
    max_rel = 0.0
    worst_col = None
    for col in shared:
        b = bcols[col]
        p = pcols[col]
        if len(b) != len(p):
            return None, None, f"row count differs in {col} ({len(b)} vs {len(p)})"
        for xb, xp in zip(b, p):
            if math.isnan(xb) and math.isnan(xp):
                continue
            if math.isnan(xb) or math.isnan(xp):
                return None, None, f"NaN mismatch in {col}"
            diff = abs(xb - xp)
            denom = max(abs(xb), abs(xp), 1e-12)
            rel = diff / denom
            if diff > max_abs:
                max_abs = diff
                worst_col = col
            max_rel = max(max_rel, rel)
    note_parts = []
    if only_base:
        note_parts.append(f"cols only in baseline: {', '.join(only_base)}")
    if only_pr:
        note_parts.append(f"cols only in candidate: {', '.join(only_pr)}")
    if worst_col:
        note_parts.append(f"worst: {worst_col}")
    return max_abs, max_rel, "; ".join(note_parts)


def fmt(x, spec="{:.3f}"):
    return "n/a" if x is None else spec.format(x)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base-dir", required=True)
    ap.add_argument("--pr-dir", required=True)
    ap.add_argument("--base-label", default="baseline")
    ap.add_argument("--pr-label", default="candidate")
    ap.add_argument("--meta", default="", help="path to a JSON metadata header")
    ap.add_argument("--out", required=True)
    args = ap.parse_args()

    base_dir = Path(args.base_dir)
    pr_dir = Path(args.pr_dir)

    base_fp = load_jsonl(base_dir / "compile.jsonl")
    pr_fp = load_jsonl(pr_dir / "compile.jsonl")
    base_idx = load_index(base_dir / "run_index.json")
    pr_idx = load_index(pr_dir / "run_index.json")

    meta = {}
    if args.meta and Path(args.meta).exists():
        meta = json.loads(Path(args.meta).read_text())

    all_keys = sorted(set(base_idx) | set(pr_idx))

    def ok(idx, key):
        r = idx.get(key)
        return bool(r) and not r["timed_out"] and r["returncode"] == 0

    lines = []
    lines.append("# RTC-Tools × pymoca benchmark")
    lines.append("")
    lines.append(
        f"Comparing pymoca **{args.base_label}** (RTC-Tools' current pin) against "
        f"**{args.pr_label}** (the `fix-inherited-symbol-scope-pr` branch), driving "
        f"the RTC-Tools example suite through the CasADi backend."
    )
    lines.append("")

    # --- Headline ---
    n = len(all_keys)
    base_ok = sum(1 for k in all_keys if ok(base_idx, k))
    pr_ok = sum(1 for k in all_keys if ok(pr_idx, k))
    both_ok = sum(1 for k in all_keys if ok(base_idx, k) and ok(pr_idx, k))
    lines.append("## Headline")
    lines.append("")
    lines.append(f"- Scripts run: **{n}**")
    lines.append(f"- Succeeded under **{args.base_label}**: **{base_ok}/{n}**")
    lines.append(f"- Succeeded under **{args.pr_label}**: **{pr_ok}/{n}**")
    lines.append(f"- Succeeded under **both** (comparable): **{both_ok}/{n}**")
    lines.append("")
    if meta:
        lines.append("## Environment")
        lines.append("")
        for k, v in meta.items():
            lines.append(f"- **{k}**: {v}")
        lines.append("")

    # --- Compile time ---
    lines.append("## Pymoca compile time (cold flatten, cache disabled)")
    lines.append("")
    lines.append(f"| Example (script) | {args.base_label} (s) | {args.pr_label} (s) | ratio (cand/base) |")
    lines.append("| --- | ---: | ---: | ---: |")
    for key in all_keys:
        b = base_fp.get(key)
        p = pr_fp.get(key)
        if not b and not p:
            continue
        bt = b["compile_seconds"] if b else None
        pt = p["compile_seconds"] if p else None
        ratio = (pt / bt) if (bt and pt and bt > 0) else None
        lines.append(f"| {key} | {fmt(bt)} | {fmt(pt)} | {fmt(ratio, '{:.2f}x')} |")
    lines.append("")

    # --- Fingerprint ---
    lines.append("## Model structure comparison")
    lines.append("")
    lines.append("| Example | states | alg_states | inputs | outputs | parameters | equations | match |")
    lines.append("| --- | ---: | ---: | ---: | ---: | ---: | ---: | :---: |")
    count_attrs = [
        ("states_count", "states"),
        ("alg_states_count", "alg_states"),
        ("inputs_count", "inputs"),
        ("outputs_count", "outputs"),
        ("parameters_count", "parameters"),
        ("equations_count", "equations"),
    ]
    name_attrs = ["states", "der_states", "alg_states", "inputs", "outputs", "constants", "parameters"]
    fp_notes = []
    for key in all_keys:
        b = base_fp.get(key)
        p = pr_fp.get(key)
        if not b or not p:
            continue
        bfp = b["fingerprint"]
        pfp = p["fingerprint"]
        cells = []
        struct_match = True
        for ckey, _label in count_attrs:
            bv = bfp.get(ckey)
            pv = pfp.get(ckey)
            if bv == pv:
                cells.append(str(bv))
            else:
                cells.append(f"{bv}→{pv}")
                struct_match = False
        # name-set diffs
        name_diffs = []
        for attr in name_attrs:
            bset = set(bfp.get(attr, []) or [])
            pset = set(pfp.get(attr, []) or [])
            if bset != pset:
                struct_match = False
                miss = sorted(bset - pset)
                extra = sorted(pset - bset)
                name_diffs.append(f"{attr}: -{miss} +{extra}")
        mark = "✅" if struct_match else "⚠️"
        lines.append(f"| {key} | " + " | ".join(cells) + f" | {mark} |")
        if name_diffs:
            fp_notes.append(f"- **{key}**: " + "; ".join(name_diffs))
    lines.append("")
    if fp_notes:
        lines.append("Name-set differences:")
        lines.append("")
        lines.extend(fp_notes)
        lines.append("")

    # --- Numerical results ---
    lines.append("## Numerical results (timeseries_export.csv)")
    lines.append("")
    lines.append(
        "Only rows where the script **succeeded under both versions** are a valid "
        "comparison; a failed run leaves stale output on disk, so those are skipped."
    )
    lines.append("")
    lines.append("| Example (script) | max abs diff | max rel diff | note |")
    lines.append("| --- | ---: | ---: | --- |")
    for key in all_keys:
        if not (ok(base_idx, key) and ok(pr_idx, key)):
            continue
        bout = base_dir / "outputs" / key / "timeseries_export.csv"
        pout = pr_dir / "outputs" / key / "timeseries_export.csv"
        if not bout.exists() and not pout.exists():
            continue
        if not bout.exists() or not pout.exists():
            miss = args.base_label if not bout.exists() else args.pr_label
            lines.append(f"| {key} | n/a | n/a | no export under {miss} |")
            continue
        max_abs, max_rel, note = compare_csv(bout, pout)
        lines.append(f"| {key} | {fmt(max_abs, '{:.3e}')} | {fmt(max_rel, '{:.3e}')} | {note} |")
    lines.append("")

    # --- Run status / failures ---
    lines.append("## Run status")
    lines.append("")
    lines.append(f"| Example (script) | {args.base_label} | {args.pr_label} |")
    lines.append("| --- | :---: | :---: |")
    for key in all_keys:
        def status(idx):
            r = idx.get(key)
            if not r:
                return "—"
            if r["timed_out"]:
                return "TIMEOUT"
            return "ok" if r["returncode"] == 0 else f"FAIL(rc={r['returncode']})"
        lines.append(f"| {key} | {status(base_idx)} | {status(pr_idx)} |")
    lines.append("")

    # --- Candidate failure reasons (extracted from logs) ---
    def last_error(log_path: Path):
        if not log_path.exists():
            return None
        tail = log_path.read_text(errors="replace").splitlines()
        for line in reversed(tail):
            s = line.strip()
            if s and ("Error" in s or "Exception" in s) and ":" in s:
                return s
        return tail[-1].strip() if tail else None

    fails = [k for k in all_keys if ok(base_idx, k) and not ok(pr_idx, k)]
    if fails:
        lines.append(f"## Failures under {args.pr_label} (passing under {args.base_label})")
        lines.append("")
        lines.append("| Example (script) | error |")
        lines.append("| --- | --- |")
        for key in fails:
            r = pr_idx.get(key, {})
            log = pr_dir / r.get("log", f"{key}.log")
            err = last_error(log) or "(see log)"
            lines.append(f"| {key} | {err} |")
        lines.append("")

    Path(args.out).write_text("\n".join(lines) + "\n")
    print(f"wrote {args.out}")


if __name__ == "__main__":
    main()
