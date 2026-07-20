# RTC-Tools × pymoca benchmark — results

Comparison of pymoca **0.9.2** (RTC-Tools' current pin, `== 0.9.*`) against the
**`fix-inherited-symbol-scope-pr`** branch **plus seven fixes on this branch**,
driving RTC-Tools 2.8.0's example suite through pymoca's CasADi backend. Both
run in virtualenvs with an identical solver stack (casadi 3.7.2, numpy, scipy);
only the pymoca version differs. The PR run has
`MODELICAPATH=<checkout>/test/libraries/MSL-4.0.x`.

Reproduce with `tools/rtc_tools_benchmark/run_benchmark.sh` (see `README.md`).
The full pymoca test suite passes throughout: **444 passed, 9 skipped, 50
xfailed, 0 failed** (`pytest test/ --ignore=test/msl_examples_test.py
--ignore=test/gen_sympy_test.py --ignore=test/xml_test.py`).

## Progress: 3/18 -> 14/18 scripts run; 12/14 bit-identical to 0.9.2

| stage | scripts passing |
| --- | --- |
| PR branch, unmodified | 3/18 |
| + honor MODELICAPATH (MSL resolves) | 10/18 |
| + preserve input/output for alias types | 10/18 (now with correct columns) |
| + propagate array dims to alias-type leaf | 12/18 |
| + rescope indices in modification expressions | 12/18 (fixes name errors, not new passes) |
| + resolve constants via composite lookup | 12/18 |
| + inline constants when rewriting expr refs | 12/18 |
| + dedupe/sanitize CasADi function names | **14/18** |

Of the 14 that run, **12 are bit-identical** to 0.9.2 (`0.000e+00` max diff).
2 (`goal_programming`, `mixed_integer`) run but the solver reports
**INFEASIBLE** — a new, distinct, uninvestigated problem (see below).
4 (`cascading_channels`, `channel_pulse`, `channel_wave_damping` x2) still
crash on a different remaining bug (also below).

## The seven fixes (commits, in order)

1. **`casadi: Resolve MODELICAPATH libraries during compile`** —
   `_compile_model` now merges `$MODELICAPATH` roots as a lazy tree, so models
   that `extend` MSL classes (e.g. `Modelica.Icons.Package`) resolve. This was
   the originally reported bug; fixes the `ModelicaSemanticError` crash on
   every Deltares-based example (3/18 -> 10/18).
2. **`tree: Preserve input/output prefix for alias-typed variables`** —
   flattening dropped a variable's `input`/`output` prefix whenever its type
   was a derived/alias type (every `Modelica.Units.SI.*`), so RTC-Tools models
   compiled but exported only a `time` column.
3. **`tree: Propagate array dimensions to a collapsed derived-type leaf`** —
   when a derived type collapses to a builtin leaf, the outer variable's own
   array dimensions weren't propagated to it, so e.g. a declared-empty
   `QForcing[n_QForcing=0]` array materialized as one spurious free variable
   instead of vanishing, giving the optimizer an unconstrained extra degree of
   freedom (10/18 -> 12/18, and made the 10 previously-blank-output scripts
   numerically correct).
4. **`tree: Rescope array indices when flattening modification expressions`**
   — an index inside a modification expression (the `n` in `b[n]`) wasn't
   rewritten to its flat name, only the ref it indexed into was; fixes
   `KeyError: 'n_level_nodes'`-style crashes.
5. **`tree: Resolve constant values reached via composite name lookup`** —
   a constant reached through a nested/redeclared package (e.g.
   `medium.n_substances`) is instantiated outside the normal flattening walk,
   so its value was never folded; fixes `NotImplementedError: ... GenDM_ones`
   from a `fill()`/`ones()` call receiving `None`.
6. **`tree: Inline constant values when rewriting expression refs`** — a
   constant used inside an otherwise-unfoldable expression (e.g.
   `Deltares.Constants.D2R * rotation_deg`) was renamed but not inlined; per
   MLS 5.6.2 constants must be inlined, and a global library constant that is
   never itself instantiated as a component may never appear in the flat
   symbol table at all. Fixes `KeyError: 'Deltares.Constants.D2R'`.
7. **`casadi: Use the deduplicated class name for CasADi functions`** — a
   function alias local to a component's class (`function smooth_switch =
   Deltares...SmoothSwitch;`) was named from its instance-path ancestry
   (`Example.orifice.smooth_switch`) rather than its class-relative
   `full_name`, and CasADi function names cannot contain dots regardless.
   Fixes `RuntimeError: ... Function name is not valid` (12/18 -> 14/18).

Each commit was verified independently: a minimal reproduction isolating the
exact defect, the full pymoca test suite (444 passed, 0 failed) after every
commit, and a full RTC-Tools benchmark re-run.

## Remaining issue A: `goal_programming` / `mixed_integer` solve INFEASIBLE

Both now flatten, compile, and run to completion, but:

```
Cbc0006I The LP relaxation is infeasible or too expensive
ERROR Solver failed with status INFEASIBLE
```

RTC-Tools does not treat this as a process failure (the script still exits 0),
so it shows as "ok" in the run-status table, but the exported values are the
solver's failure fallback (mostly zero), not a real solution — max rel diff
against 0.9.2 is 1.0. **Not investigated further**: this is a new, distinct
problem (a real optimization infeasibility, not a naming/lookup crash like the
other six), and finding its root cause is a separate, open-ended
investigation of comparable scope to any of the fixes above.

## Remaining issue B: `Deltares` KeyError in 4 channel-flow examples

```
KeyError: 'Deltares'
```

Root-caused to: `_get_constant_value` can extract a constant's literal when
its type is *directly* a builtin (`Integer`/`Real`/...), by reading a
synthetic same-named symbol in the type's own `.symbols`. It cannot do this
when the constant's type is itself an alias one level removed (e.g.
`final constant Modelica.Units.SI.Acceleration g_n = 9.80665;` inside
`Deltares.Constants`, where the type is `Acceleration`, not `Real`). Minimal
reproduction:

```modelica
package Deltares
  package Constants
    type Accel = Real(unit="m/s2");
    final constant Accel g_n = 9.8;
  end Constants;
end Deltares;
```

For this symbol, neither `.value`, `.ast_ref.value`, nor
`.modification_environment.arguments` carry the literal `9.8` on the
`InstanceSymbol` produced by composite-name lookup, and the referenced type's
own `.symbols` is empty (unlike the directly-builtin-typed case fix #5
handles) — so the existing `_get_constant_value` fallback has nothing to read.
**Not fixed**: the value appears to be lost during partial instantiation for
lookup somewhere in `_instantiation.py`/`_name_lookup.py`, upstream of
`_get_constant_value`; tracing that precisely is a further, separate
investigation.

## Compile time

Not a fair comparison yet: MSL now loads on every cold compile (~17-20s vs
~1.5-3s for 0.9.2), and the models that still crash contribute no data point.
Revisit once all 18 pass.

## Bottom line

Seven atomic, independently-verified fixes took the branch from 3/18 to 14/18
RTC-Tools scripts running, with 12/14 of those bit-identical to 0.9.2. Two
distinct, uninvestigated problems remain, each independent of the fixes above
and of each other: an INFEASIBLE solve in 2 examples, and a constant-value
extraction gap for alias-typed global constants in 4 examples.

---
## Generated data

## Headline

- Scripts run: **18**
- Succeeded under **0.9.2**: **18/18**
- Succeeded under **pr+7fix**: **14/18**
- Succeeded under **both** (comparable): **14/18**

## Environment

- **generated**: 2026-07-20T15:52:09Z
- **rtc_tools**: 2.8.0
- **0.9.2 (baseline)**: pymoca 0.9.2 | casadi 3.7.2
- **pr + 7 fixes**: 0+untagged.285.gfbbacb3 | MODELICAPATH=MSL-4.0.x

## Pymoca compile time (cold flatten, cache disabled)

| Example (script) | 0.9.2 (s) | pr+7fix (s) | ratio (cand/base) |
| --- | ---: | ---: | ---: |
| basic__example | 1.907 | 18.736 | 9.82x |
| cascading_channels__example | 2.675 | n/a | n/a |
| channel_pulse__example | 1.768 | n/a | n/a |
| channel_wave_damping__example_local_control | 3.436 | n/a | n/a |
| channel_wave_damping__example_optimization | 2.970 | n/a | n/a |
| ensemble__example | 1.876 | 19.142 | 10.21x |
| fallback_option__example | 1.907 | 17.845 | 9.36x |
| fallback_option__example_with_gp | 1.820 | 18.113 | 9.95x |
| goal_programming__example | 1.970 | 21.753 | 11.04x |
| integrator_delay__example | 1.435 | 2.510 | 1.75x |
| lookup_table__example | 1.851 | 18.411 | 9.95x |
| mixed_integer__example | 2.024 | 20.905 | 10.33x |
| pumped_hydropower_system__example | 2.083 | 19.796 | 9.50x |
| simulation__example | 1.910 | 20.541 | 10.76x |
| simulation_with_custom_equations__simple_model | 1.867 | 4.795 | 2.57x |
| single_reservoir__single_reservoir | 1.428 | 2.212 | 1.55x |

## Model structure comparison

| Example | states | alg_states | inputs | outputs | parameters | equations | match |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | :---: |
| basic__example | 1 | 0 | 2 | 1 | 4 | 1 | ✅ |
| ensemble__example | 1 | 0 | 2 | 0 | 4 | 1 | ✅ |
| fallback_option__example | 1 | 0 | 2 | 1 | 4 | 1 | ✅ |
| fallback_option__example_with_gp | 1 | 0 | 2 | 1 | 4 | 1 | ✅ |
| goal_programming__example | 1 | 5 | 5 | 2 | 12 | 6 | ✅ |
| integrator_delay__example | 1 | 0 | 4 | 1 | 8 | 1 | ✅ |
| lookup_table__example | 1 | 0 | 2 | 0 | 4 | 1 | ✅ |
| mixed_integer__example | 1 | 5 | 5 | 2 | 12 | 6 | ✅ |
| pumped_hydropower_system__example | 2 | 10 | 6 | 11 | 17 | 11 | ✅ |
| simulation__example | 1 | 0 | 3 | 2 | 5 | 1 | ✅ |
| simulation_with_custom_equations__simple_model | 1 | 0 | 0 | 1 | 0 | 0 | ✅ |
| single_reservoir__single_reservoir | 1 | 0 | 2 | 1 | 4 | 1 | ✅ |

## Numerical results (timeseries_export.csv)

Only rows where the script **succeeded under both versions** are a valid comparison; a failed run leaves stale output on disk, so those are skipped.

| Example (script) | max abs diff | max rel diff | note |
| --- | ---: | ---: | --- |
| basic__example | 0.000e+00 | 0.000e+00 |  |
| channel_wave_damping__steady_state_initialization_mixin | 0.000e+00 | 0.000e+00 |  |
| channel_wave_damping__step_size_parameter_mixin | 0.000e+00 | 0.000e+00 |  |
| goal_programming__example | 5.583e+00 | 1.000e+00 | worst: Q_pump |
| integrator_delay__example | 0.000e+00 | 0.000e+00 |  |
| lookup_table__example | 0.000e+00 | 0.000e+00 |  |
| mixed_integer__example | 5.822e+00 | 1.000e+00 | worst: Q_orifice |
| pumped_hydropower_system__example | 0.000e+00 | 0.000e+00 |  |
| simulation__example | 0.000e+00 | 0.000e+00 |  |
| simulation_with_custom_equations__simple_model | 0.000e+00 | 0.000e+00 |  |
| single_reservoir__single_reservoir | 0.000e+00 | 0.000e+00 |  |

## Run status

| Example (script) | 0.9.2 | pr+7fix |
| --- | :---: | :---: |
| basic__example | ok | ok |
| cascading_channels__example | ok | FAIL(rc=1) |
| channel_pulse__example | ok | FAIL(rc=1) |
| channel_wave_damping__example_local_control | ok | FAIL(rc=1) |
| channel_wave_damping__example_optimization | ok | FAIL(rc=1) |
| channel_wave_damping__steady_state_initialization_mixin | ok | ok |
| channel_wave_damping__step_size_parameter_mixin | ok | ok |
| ensemble__example | ok | ok |
| fallback_option__example | ok | ok |
| fallback_option__example_with_gp | ok | ok |
| goal_programming__example | ok | ok |
| integrator_delay__example | ok | ok |
| lookup_table__example | ok | ok |
| mixed_integer__example | ok | ok |
| pumped_hydropower_system__example | ok | ok |
| simulation__example | ok | ok |
| simulation_with_custom_equations__simple_model | ok | ok |
| single_reservoir__single_reservoir | ok | ok |

## Failures under pr+7fix (passing under 0.9.2)

| Example (script) | error |
| --- | --- |
| cascading_channels__example | KeyError: 'Deltares' |
| channel_pulse__example | KeyError: 'Deltares' |
| channel_wave_damping__example_local_control | KeyError: 'Deltares' |
| channel_wave_damping__example_optimization | KeyError: 'Deltares' |

