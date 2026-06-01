"""Process every Example model in the Modelica Standard Library

By default each model is run through the new flatten pipeline (tree.flatten_model).
Pass -t/--translator casadi to instead translate each model with the CasADi backend.
"""

import gc
import os
import subprocess
import sys
import time
import traceback
from multiprocessing import Pool
from pathlib import Path

from pymoca import ast
from pymoca import parser
from pymoca import tree

MY_DIR = os.path.dirname(os.path.realpath(__file__))
TEST_DIR = os.path.join(MY_DIR, "..", "test")
MSL4_BASE_DIR = os.path.join(TEST_DIR, "libraries", "MSL-4.0.x")

# Worker-process state (set once by _worker_init, reused for all tasks).
# _worker_tree is None in fresh-tree mode: each task parses its own tree.
_worker_tree = None
_use_legacy = False
_msl4_base_dir = None
_translator = None
_options = None
# CasADi modules, imported lazily by _worker_init only when translating.
_casadi_generator = None
_casadi_api = None


def _worker_init(
    msl4_base_dir: str,
    use_legacy: bool = False,
    reuse_tree: bool = False,
    translator: str = None,
    options: dict = None,
) -> None:
    global _worker_tree, _use_legacy, _msl4_base_dir, _translator, _options
    global _casadi_generator, _casadi_api
    _msl4_base_dir = msl4_base_dir
    _use_legacy = use_legacy
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
        worker_tree = parser.modelicapath_to_tree([_msl4_base_dir])
    gc.collect()
    vsz_before = _vsz_mb()
    t0 = time.perf_counter()
    try:
        if _translator == "casadi":
            # Replicate the compile tail of casadi.api._compile_model against the
            # shared/fresh tree so that --reuse-tree is honored (transfer_model
            # reparses a folder on every call and cannot reuse a tree).
            opts = _casadi_api._merge_default_options(_options)
            model = _casadi_generator.generate(worker_tree, model_name, opts)
            if opts["check_balanced"]:
                model.check_balanced()
            model.simplify(opts)
            model._post_checks()
            name = model_name
        elif _use_legacy:
            flat_class = ast.ComponentRef.from_string(model_name)
            flat_tree = tree.flatten(worker_tree, flat_class)
            name = flat_tree.classes[model_name].name
        else:
            flat_instance = tree.flatten_model(worker_tree, model_name)
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
    filters: list = None,
    legacy: bool = False,
    reuse_tree: bool = False,
    translator: str = None,
    options: dict = None,
):
    msl_path = Path(MSL4_BASE_DIR) / "Modelica"
    root_index = len(msl_path.parts) - 1
    model_names = sorted(
        ".".join(p.parts[root_index:-1] + (p.stem,))
        for p in msl_path.glob("**/Examples/**/*.mo")
        if p.name != "package.mo"
    )
    if filters:
        model_names = [n for n in model_names if any(f in n for f in filters)]

    num_success = 0
    num_error = 0
    wall_t0 = time.perf_counter()

    if jobs == 1:
        # Serial: initialize worker state in-process so ordering bugs remain reproducible.
        _worker_init(MSL4_BASE_DIR, legacy, reuse_tree, translator, options)
        results = map(_process_one, model_names)
    else:
        pool = Pool(
            processes=jobs,
            initializer=_worker_init,
            initargs=[MSL4_BASE_DIR, legacy, reuse_tree, translator, options],
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
        if jobs > 1:
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
        "-l",
        "--legacy",
        action="store_true",
        default=False,
        help="use the legacy tree.flatten() pipeline instead of flatten_model()",
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

    options, opt_errors = build_define_options(args)
    if opt_errors:
        sys.exit(2)
    if args.define and not args.translator:
        ap.error("-D/--define requires -t/--translator")

    process_every_MSL_example(
        jobs=args.jobs,
        filters=args.filters,
        legacy=args.legacy,
        reuse_tree=args.reuse_tree,
        translator=args.translator,
        options=options,
    )
    sys.exit(0)
