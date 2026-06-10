"""Pytest parametrized tests and CLI for MSL-4.0.x Example models

By default each model is run through the new flatten pipeline (tree.flatten_class).
Pass -t/--translator casadi to instead translate each model with the CasADi backend.
"""

from __future__ import annotations

import gc
import os
import subprocess
import sys
import time
import traceback
from multiprocessing import Pool
from pathlib import Path

from pymoca import parser
from pymoca import tree

import pytest  # type: ignore[import-untyped]

MY_DIR = os.path.dirname(os.path.realpath(__file__))
MSL4_BASE_DIR = os.path.join(MY_DIR, "libraries", "MSL-4.0.x")
MSL4_AVAILABLE = os.path.isfile(os.path.join(MSL4_BASE_DIR, "Modelica", "package.mo"))

# Maps model name → reason string for models known to fail.
KNOWN_FAILURES = {}

# ---------------------------------------------------------------------------
# Discovery
# ---------------------------------------------------------------------------


def _discover_model_names():
    msl_path = Path(MSL4_BASE_DIR) / "Modelica"
    root_index = len(msl_path.parts) - 1
    return sorted(
        ".".join(p.parts[root_index:-1] + (p.stem,))
        for p in msl_path.glob("**/Examples/**/*.mo")
        if p.name != "package.mo"
    )


def _build_params():
    params = []
    for name in _discover_model_names():
        marks = []
        if name in KNOWN_FAILURES:
            marks.append(pytest.mark.xfail(reason=KNOWN_FAILURES[name]))
        params.append(pytest.param(name, id=name, marks=marks))
    return params


# ---------------------------------------------------------------------------
# Pytest tests
# ---------------------------------------------------------------------------

pytestmark = pytest.mark.skipif(not MSL4_AVAILABLE, reason="MSL-4.0.x submodule not initialized")


# Use scope="session" to reuse one tree (should be faster, but currently too memory-hungry)
@pytest.fixture(scope="function")
def msl_tree():
    return parser.modelicapath_to_tree([MSL4_BASE_DIR])


@pytest.mark.msl
@pytest.mark.parametrize("model_name", _build_params() if MSL4_AVAILABLE else [])
def test_msl_example(model_name, msl_tree):
    flat_instance = tree.flatten_class(msl_tree, model_name)
    assert flat_instance is not None


# ---------------------------------------------------------------------------
# CLI worker state (set once by _worker_init, reused for all tasks)
# _worker_tree is None in fresh-tree mode: each task parses its own tree.
# ---------------------------------------------------------------------------

_worker_tree = None
_msl4_base_dir = None
_translator = None
_options = None
# CasADi modules, imported lazily by _worker_init only when translating.
_casadi_generator = None
_casadi_api = None


def _worker_init(
    msl4_base_dir: str,
    reuse_tree: bool = False,
    translator: str | None = None,
    options: dict | None = None,
) -> None:
    global _worker_tree, _msl4_base_dir, _translator, _options
    global _casadi_generator, _casadi_api
    _msl4_base_dir = msl4_base_dir
    _translator = translator
    _options = options or {}
    if translator == "casadi":
        # Importing CasADi (and the generator) is slow; only do it when needed.
        from pymoca.backends.casadi import api as _api
        from pymoca.backends.casadi import generator as _gen

        _casadi_api = _api
        _casadi_generator = _gen
    if reuse_tree:
        _worker_tree = parser.modelicapath_to_tree([msl4_base_dir])


def _vsz_mb() -> float:
    """Current virtual memory size in MB."""
    if sys.platform == "linux":
        with open("/proc/self/status") as f:
            for line in f:
                if line.startswith("VmSize:"):
                    return int(line.split()[1]) / 1024.0
        return 0.0
    # macOS/BSD: ps -o vsz gives current VSZ in kB
    out = subprocess.check_output(["ps", "-o", "vsz=", "-p", str(os.getpid())])
    return int(out.strip()) / 1024.0


def _process_one(model_name: str) -> tuple:
    """Process one model; return (status, message, elapsed_s, delta_vsz_mb)."""
    verb = "translating" if _translator == "casadi" else "flattening"
    worker_tree = _worker_tree
    if worker_tree is None:
        worker_tree = parser.modelicapath_to_tree([_msl4_base_dir])  # type: ignore[list-item]
    gc.collect()
    vsz_before = _vsz_mb()
    t0 = time.perf_counter()
    try:
        if _translator == "casadi":
            # Replicate the compile tail of casadi.api._compile_model against the
            # shared/fresh tree so that --reuse-tree is honored (transfer_model
            # reparses a folder on every call and cannot reuse a tree).
            assert _casadi_api is not None and _casadi_generator is not None
            opts = _casadi_api._merge_default_options(_options)
            model = _casadi_generator.generate(worker_tree, model_name, opts)
            if opts["check_balanced"]:
                model.check_balanced()
            model.simplify(opts)
            model._post_checks()
            name = model_name
        else:
            flat_instance = tree.flatten_class(worker_tree, model_name)
            name = flat_instance.name
        elapsed = time.perf_counter() - t0
        delta_vsz = _vsz_mb() - vsz_before
        return ("success", f"Success {verb} {name}", elapsed, delta_vsz)
    except parser.ModelicaSyntaxError as exc:
        elapsed = time.perf_counter() - t0
        delta_vsz = _vsz_mb() - vsz_before
        import io

        buf = io.StringIO()
        parser.print_syntax_error(exc, buf)
        return (
            "error",
            f"Error parsing (syntax error) {model_name}:\n{buf.getvalue()}",
            elapsed,
            delta_vsz,
        )
    except NotImplementedError as exc:
        elapsed = time.perf_counter() - t0
        delta_vsz = _vsz_mb() - vsz_before
        return (
            "error",
            f"Error parsing (parser error) {model_name}: {exc}",
            elapsed,
            delta_vsz,
        )
    except tree.ModelicaError as exc:
        elapsed = time.perf_counter() - t0
        delta_vsz = _vsz_mb() - vsz_before
        return ("error", f"Error {verb} {model_name}: {exc}", elapsed, delta_vsz)
    except Exception:
        elapsed = time.perf_counter() - t0
        delta_vsz = _vsz_mb() - vsz_before
        tb = traceback.format_exc()
        return ("error", f"Error {verb} {model_name}:\n{tb}", elapsed, delta_vsz)


def _default_jobs() -> int:
    # multiprocessing not helpful currently due to excessive virtual memory used
    # return max(1, (os.cpu_count() or 1) * 3 // 4)
    return 1


def process_every_MSL_example(
    jobs: int = 1,
    filters: list | None = None,
    reuse_tree: bool = False,
    translator: str | None = None,
    options: dict | None = None,
):
    model_names = _discover_model_names()
    if filters:
        model_names = [n for n in model_names if any(f in n for f in filters)]

    num_success = 0
    num_error = 0
    wall_t0 = time.perf_counter()

    pool = None
    if jobs == 1:
        # Serial: initialize worker state in-process so ordering bugs remain reproducible.
        _worker_init(MSL4_BASE_DIR, reuse_tree, translator, options)
        results = map(_process_one, model_names)
    else:
        pool = Pool(
            processes=jobs,
            initializer=_worker_init,
            initargs=[MSL4_BASE_DIR, reuse_tree, translator, options],
        )
        results = pool.imap_unordered(_process_one, model_names)

    total_models = len(model_names)
    done = 0
    try:
        for status, message, elapsed, delta_vsz in results:
            done += 1
            suffix = f"  [{elapsed:.2f}s {delta_vsz:+.0f}MB]"
            print(message + suffix, flush=True)
            print(f"[{done}/{total_models}]", end="\r", file=sys.stderr, flush=True)
            if status == "success":
                num_success += 1
            else:
                num_error += 1
    finally:
        if pool is not None:
            pool.close()
            pool.join()

    wall_elapsed = time.perf_counter() - wall_t0
    total = num_success + num_error
    pct = num_success / total * 100 if total else 0.0
    verb = "translating" if translator == "casadi" else "flattening"
    print("==================================================================================")
    print(f"Success {verb} {num_success} of {total} ({pct:.2f}%)  wall time: {wall_elapsed:.1f}s")


if __name__ == "__main__":
    import argparse

    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument(
        "-j",
        "--jobs",
        type=int,
        default=_default_jobs(),
        metavar="N",
        help=f"worker processes (default: {_default_jobs()}; 1 = serial)",
    )
    ap.add_argument(
        "-f",
        "--filter",
        dest="filters",
        action="append",
        metavar="PATTERN",
        help="only run models whose name contains PATTERN (repeatable, OR logic)",
    )
    ap.add_argument(
        "--reuse-tree",
        action="store_true",
        default=False,
        help="reuse a single parsed tree across all models (faster, but uses more memory)",
    )
    ap.add_argument(
        "-t",
        "--translator",
        dest="translator",
        choices=("casadi",),
        default=None,
        help="translate each model with this backend instead of only flattening",
    )
    ap.add_argument(
        "-D",
        "--define",
        dest="define",
        action="append",
        metavar="NAME=VALUE",
        help="translator option in the form NAME=VALUE (repeatable; only with -t)",
    )
    args = ap.parse_args()

    # Reuse the CLI's option parser so -D semantics match `pymoca -D ...`.
    from pymoca.compiler import build_define_options

    cli_options, opt_errors = build_define_options(args)
    if opt_errors:
        sys.exit(2)
    if args.define and not args.translator:
        ap.error("-D/--define requires -t/--translator")

    process_every_MSL_example(
        jobs=args.jobs,
        filters=args.filters,
        reuse_tree=args.reuse_tree,
        translator=args.translator,
        options=cli_options,
    )
    sys.exit(0)
