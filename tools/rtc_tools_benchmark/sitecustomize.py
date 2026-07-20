"""Auto-imported shim that instruments pymoca's CasADi ``transfer_model``.

Python imports ``sitecustomize`` automatically at interpreter startup when it is
found on ``sys.path``.  The benchmark driver puts this directory at the front of
``PYTHONPATH`` for every example subprocess, so ``transfer_model`` gets wrapped
without touching RTC-Tools or the example scripts.

The wrapper:

* forces ``cache=False`` so every call performs a cold flatten (the real
  "pymoca time"), never a cached-model load;
* times the (cold) call with ``perf_counter``;
* records a structural *fingerprint* of the returned model (variable and
  equation counts plus sorted variable names) so the two pymoca versions can be
  compared for equivalence;
* appends one JSON record per call to the file named by ``$BENCH_JSON``.

Everything is controlled through environment variables so the same shim works
for any pymoca / RTC-Tools version:

    BENCH_JSON          path to the JSONL results file (appended to)
    BENCH_EXAMPLE       label for the example/script being run
    BENCH_PYMOCA_VER    label for the pymoca version under test
"""

import functools
import json
import os
import time


def _symbol_names(elements):
    names = []
    for elem in elements:
        name = None
        sym = getattr(elem, "symbol", None)
        if sym is not None:
            try:
                name = sym.name()
            except Exception:
                name = None
        if name is None:
            name = getattr(elem, "name", None)
            if callable(name):
                try:
                    name = name()
                except Exception:
                    name = None
        if name is None:
            name = str(elem)
        names.append(str(name))
    return sorted(names)


def _fingerprint(model):
    fp = {}
    for attr in (
        "states",
        "der_states",
        "alg_states",
        "inputs",
        "outputs",
        "constants",
        "parameters",
    ):
        try:
            elements = list(getattr(model, attr))
            fp[attr + "_count"] = len(elements)
            fp[attr] = _symbol_names(elements)
        except Exception as exc:  # pragma: no cover - defensive
            fp[attr + "_count"] = None
            fp[attr + "_error"] = repr(exc)
    for attr in ("equations", "initial_equations"):
        try:
            fp[attr + "_count"] = len(getattr(model, attr))
        except Exception as exc:  # pragma: no cover - defensive
            fp[attr + "_count"] = None
            fp[attr + "_error"] = repr(exc)
    return fp


def _install():
    try:
        import pymoca.backends.casadi.api as api
    except Exception:
        return

    original = api.transfer_model
    json_path = os.environ.get("BENCH_JSON")
    example = os.environ.get("BENCH_EXAMPLE", "?")
    version = os.environ.get("BENCH_PYMOCA_VER", "?")

    @functools.wraps(original)
    def wrapper(model_folder, model_name, compiler_options=None):
        options = dict(compiler_options or {})
        options["cache"] = False  # always cold-compile for a fair time comparison
        options.pop("codegen", None)

        start = time.perf_counter()
        model = original(model_folder, model_name, options)
        elapsed = time.perf_counter() - start

        record = {
            "example": example,
            "version": version,
            "model_folder": str(model_folder),
            "model_name": model_name,
            "compile_seconds": elapsed,
            "fingerprint": _fingerprint(model),
        }
        if json_path:
            try:
                with open(json_path, "a") as handle:
                    handle.write(json.dumps(record) + "\n")
            except Exception:
                pass
        return model

    api.transfer_model = wrapper


_install()
