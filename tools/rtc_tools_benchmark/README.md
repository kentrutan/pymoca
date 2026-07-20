# RTC-Tools × pymoca benchmark

Compares two pymoca versions by driving the [RTC-Tools](https://github.com/rtc-tools/rtc-tools)
example suite through pymoca's CasADi backend, measuring **pymoca compile time**
(cold flatten), the **structure of the flattened model**, and the **numerical
results** of each example.

It exists to validate large pymoca changes (such as the
`fix-inherited-symbol-scope-pr` tree/flattening rewrite) against a real
downstream consumer, and is written to stay useful for *future* versions of both
pymoca and RTC-Tools — every version is a parameter, nothing is hard-coded.

## One-command usage

```bash
tools/rtc_tools_benchmark/run_benchmark.sh \
    --pymoca-a 0.9.2                          --label-a 0.9.2 \
    --pymoca-b /path/to/pymoca-branch-checkout --label-b pr
```

- `--pymoca-a` / `--pymoca-b` each accept **either** a pip spec (`0.9.2`,
  `pymoca==0.9.2`) **or** a path to a pymoca checkout (installed editable,
  `--no-deps`).
- `--rtctools` selects the RTC-Tools release (default: latest `rtc-tools` on
  PyPI). Pass any spec, e.g. `--rtctools rtc-tools==2.8.0`.
- `--rtc-src` points at an existing RTC-Tools checkout (for its `examples/`);
  omit to shallow-clone `master`.
- `--only basic,mixed_integer` restricts to named examples; `--timeout` bounds
  each script (default 900 s); `--workdir` sets where venvs/results/report go.
- `--modelicapath <dir>` puts a library root (e.g. an MSL) on `MODELICAPATH`
  for every run. When omitted and `--pymoca-b` is a checkout, the driver
  initializes and uses that checkout's vendored `test/libraries/MSL-4.0.x`
  submodule. The RTC-Tools Deltares models `extend` MSL classes such as
  `Modelica.Icons.Package`; the patched CasADi backend reads `MODELICAPATH` so
  they resolve, and pymoca versions that don't read it simply ignore the var.

Output: `<workdir>/RESULTS.md`.

## How it works

- Two throwaway virtualenvs are built with an **identical** RTC-Tools + solver
  stack (casadi/numpy/scipy pinned by RTC-Tools); only the pymoca version
  differs, installed with `--no-deps` so nothing else moves.
- `sitecustomize.py` is placed on each subprocess's `PYTHONPATH`. Python
  auto-imports it and it wraps `pymoca.backends.casadi.api.transfer_model` to
  force `cache=False` (cold flatten), time the call, and record a structural
  fingerprint. Nothing in RTC-Tools or the examples is edited.
- `run_all.py` discovers every `examples/*/src/*.py`, cleans **both** caches
  before each run — RTC-Tools' `*.pymoca_cache` compiled-model files **and**
  pymoca's own parse cache (relocated to a per-run `XDG_CACHE_HOME` that is
  wiped) — runs the script, and snapshots its `output/` folder.
- `compare.py` reads both versions' results and writes `RESULTS.md`.

## Repeating for future versions

The whole thing is version-agnostic; just change the refs:

```bash
# a future pymoca tag vs the current PR branch, on a newer RTC-Tools
run_benchmark.sh --pymoca-a 0.9.2 --pymoca-b 0.12.0 --label-b 0.12.0 \
                 --rtctools rtc-tools==2.9.0
```

## Files

| File | Role |
| --- | --- |
| `run_benchmark.sh` | parameterized driver (builds venvs, runs, reports) |
| `run_all.py` | runs the example suite under one venv, cleans caches, snapshots output |
| `sitecustomize.py` | auto-loaded shim that times & fingerprints `transfer_model` |
| `compare.py` | reads both runs, writes `RESULTS.md` |
