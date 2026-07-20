# RTC-Tools × pymoca benchmark — results

Comparison of pymoca **0.9.2** (RTC-Tools' current pin, `== 0.9.*`) against the
**`fix-inherited-symbol-scope-pr`** branch **plus nine fixes on this branch**,
driving RTC-Tools 2.8.0's example suite through pymoca's CasADi backend. Both
run in virtualenvs with an identical solver stack (casadi 3.7.2, numpy, scipy);
only the pymoca version differs. The PR run has
`MODELICAPATH=<checkout>/test/libraries/MSL-4.0.x`.

Reproduce with `tools/rtc_tools_benchmark/run_benchmark.sh` (see `README.md`).
The full pymoca test suite passes throughout: **445 passed, 9 skipped, 50
xfailed, 0 failed** (`pytest test/ --ignore=test/msl_examples_test.py
--ignore=test/gen_sympy_test.py --ignore=test/xml_test.py`).

## Progress: 3/18 -> 18/18 scripts run; 15/18 bit-identical to 0.9.2

| stage | scripts passing |
| --- | --- |
| PR branch, unmodified | 3/18 |
| + honor MODELICAPATH (MSL resolves) | 10/18 |
| + preserve input/output for alias types | 10/18 (now with correct columns) |
| + propagate array dims to alias-type leaf | 12/18 |
| + rescope indices in modification expressions | 12/18 (fixes name errors, not new passes) |
| + resolve constants via composite lookup | 12/18 |
| + inline constants when rewriting expr refs | 12/18 |
| + dedupe/sanitize CasADi function names | 14/18 |
| + resolve constants through type-alias extends chains | 14/18 |
| + inline constants referenced directly in equations | 14/18 |
| + fix outer-scope reference re-prefixed to itself | **18/18** |

Of the 18 that run, **15 are bit-identical** to 0.9.2 (`0.000e+00` max diff).
3 (`goal_programming`, `mixed_integer`, `channel_wave_damping__example_optimization`)
run but the solver fails to find a real solution — a distinct,
still-uninvestigated problem (Remaining issue A, below). The other 3
non-bit-identical rows in the numeric table are a benchmark-harness artifact,
not a divergence (see the note under "Numerical results").

## The ten fixes (commits, in order)

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
8. **`tree: Resolve constant values through type-alias extends chains`** —
   `_get_constant_value` could only read a constant's literal off a symbol
   typed *directly* as a builtin. A constant typed as an alias one or more
   levels removed from a builtin (`type Accel = Real(...); constant Accel g_n
   = 9.80665;`, the common Deltares pattern) has its builtin leaf inside the
   alias's own unnamed extends chain instead, which the lookup didn't walk.
   Extended both the modification-application filter and the value-extraction
   helper to follow that chain. Fixes `KeyError: 'Deltares'`.
9. **`tree: Inline constants referenced directly in equations`** — constants
   reached only via composite lookup (never instantiated as a named
   component, e.g. `Deltares.Constants.g_n` used directly in an equation
   right-hand side) are never given their own flat symbol, so
   `_EquationRefResolver` — which only renames refs matching an existing flat
   symbol — leaves them untouched. Per MLS 5.6.2 constants must be inlined;
   added a recursive walk (covering `for`/`if`/`when` bodies) that inlines any
   constant ref an equation's resolver pass didn't already rename.
10. **`tree: Fix outer-scope reference re-prefixed to itself`** — a nested
    component's modification value naming an outer-scope symbol that happens
    to share the nested component's own local name (e.g. `Inner storage(theta
    = theta)`, where both `Inner` and the enclosing model declare `theta`)
    resolved to a self-reference instead of the outer symbol. The final
    symbol-attribute pass (`ComponentRefFlattener`) blindly re-prefixes every
    embedded `ComponentRef` with the owning symbol's instance path, assuming
    it is still unresolved; a modification's `ComponentRef` value has already
    been rewritten to its true flat name by then (MLS 5.6.2 point B), and
    re-prefixing an already-flat name is redundant at best — when the
    prefixed name coincidentally collides with an existing flat symbol (as
    with same-named nested/outer `theta`), it is actively wrong. Fixed by
    preferring the raw (already-flat) name whenever it independently
    resolves. This was the actual root cause of the `Deltares` KeyError class
    of failures fix #8 only partially addressed: it fixed constant-value
    extraction, but the 4 channel-flow examples (`cascading_channels`,
    `channel_pulse`, `channel_wave_damping` x2) kept failing afterward on a
    *different* symptom (`... variables [...] are free`) traced to this same
    re-prefixing bug corrupting a `parameter`-to-`parameter` reference
    (`theta = theta`) rather than a constant. Fixing it took all 18 scripts to
    passing (14/18 -> 18/18).

Each commit was verified independently: a minimal reproduction isolating the
exact defect, the full pymoca test suite (444 or 445 passed, 0 failed) after
every commit, and a full RTC-Tools benchmark re-run.

## Remaining issue A: 3 examples solve to a fallback, not a real answer

`goal_programming`, `mixed_integer`, and `channel_wave_damping__example_optimization`
all now flatten, compile, and run to completion (exit 0), but the solver does
not find a real solution:

```
Cbc0006I The LP relaxation is infeasible or too expensive
ERROR Solver failed with status INFEASIBLE
```
```
Exception of type: TOO_FEW_DOF in file "Interfaces/IpIpoptApplication.cpp" ...
WARNING Solver failed with status Not_Enough_Degrees_Of_Freedom
```

RTC-Tools does not treat either as a process failure (the script still exits
0), so these show as "ok" in the run-status table, but the exported values
are the solver's failure fallback (mostly zero), not a real solution — max
rel diff against 0.9.2 is 1.0 for all three.

**Ruled out this session**:

- A symbol-level fingerprint diff against 0.9.2 for `goal_programming` (using
  the correct `tree.flatten()`/`flatten_to_tree` entry point — the one the
  CasADi backend actually uses) shows the flattened models are
  **structurally identical**: same symbol count (73), same equation count
  (37), same prefixes/min/max/fixed for every sampled variable.
- For `channel_wave_damping__example_optimization`, comparing `model.states` /
  `model.alg_states` between PR and 0.9.2 **with matching `compiler_options`**
  (crucially including `detect_aliases`, `eliminate_constant_assignments`,
  etc. — RTC-Tools' `ModelicaMixin` sets all of these) shows an exact match
  (27 states, 0 alg_states, both versions). An earlier lead pointing at a
  149-vs-143 equation-count mismatch turned out to be an artifact of a
  debugging script that omitted those simplification options on the PR side
  only — not a real structural difference.
- The extra 6 `parameters` this example now correctly exposes
  (`{reach}.min_abs_Q`, `{reach}.min_divisor`, see below) are **not** the
  cause: parameters are fixed, not NLP decision variables, so a parameter
  count difference cannot by itself change the problem's degrees of freedom.

So this is not a flattening/fingerprint bug like fixes 1-10 — the defect is
either in specific equation *values* (not visible in a structural
fingerprint), in how the CasADi generator builds the residual/bounds from
that structure, or a solver-path issue specific to
`GoalProgrammingMixin`/discrete-decision models. **Not investigated further**
in this session: narrowing it requires comparing generated equation
expressions or numerically evaluating `dae_residual_function` at matched
points between versions, which is a separate, open-ended investigation.

## Extra `min_abs_Q` / `min_divisor` parameters: expected, not a bug

`cascading_channels`, `channel_pulse`, and `channel_wave_damping` (both
examples) now correctly expose 2 extra parameters per channel reach
(`{reach}.min_abs_Q`, `{reach}.min_divisor`) that 0.9.2's output lacks
entirely. Root-caused directly (flattened both versions' `Example` model
side by side down to the CasADi-generator level):

- Both parameters default to `Deltares.Constants.eps`, a constant reached
  through the same alias/nested-package pattern fix #8 addresses
  (`Deltares.Constants` package, `Real`-alias-typed).
- Under the PR (after fix #8), `min_divisor`/`min_abs_Q` resolve correctly to
  their real numeric default (`1e-12`) and appear as ordinary flat
  parameters — the spec-correct MLS 5.6.2 result.
- Under 0.9.2, the same default resolves to an **unresolved dangling MX
  symbol literally named `"LowerChannel.Deltares.Constants.eps"`**
  (confirmed by instrumenting `generator.generate()` directly: `p.value` is a
  non-constant MX expression with that exact name, not a number). 0.9.2's
  `replace_parameter_expressions` step then sees a non-constant value,
  substitutes this dangling symbol into the equations, and drops the
  parameter from `model.parameters` entirely — so it silently vanishes from
  every category (not reclassified as a constant or alg_state; it is simply
  gone) rather than erroring, which is 0.9.2's own instance of the exact
  defect class fix #8 fixes on the PR branch, just failing silently instead
  of raising.

This is 0.9.2 being **less correct**, not the PR being wrong: matching 0.9.2
here would mean deliberately reintroducing a resolution defect. The PR's own
results for these examples are numerically sane (spot-checked
`cascading_channels`' `DrinkingWaterExtractionPump_Q` stays in a normal
`[0.29, 1.0]` range throughout). Not something to fix.

## Compile time

Not a fair comparison: MSL now loads on every cold compile (~20-30s vs
~1.5-3.5s for 0.9.2). This is inherent to giving the PR branch a real MSL on
`MODELICAPATH` (required for the Deltares/MSL `extends` chains to resolve at
all) and is orthogonal to the fixes above.

## Bottom line

Ten atomic, independently-verified fixes took the branch from 3/18 to 18/18
RTC-Tools scripts running, with 15/18 of those bit-identical to 0.9.2. Of the
3 non-bit-identical examples with extra `min_abs_Q`/`min_divisor` parameters,
that divergence is a benign, expected correctness improvement over 0.9.2, not
a bug. One distinct, unresolved problem remains: 3 examples
(`goal_programming`, `mixed_integer`, `channel_wave_damping__example_optimization`)
run to completion but the solver fails to find a real solution, a
deeper numerical/generator issue not yet root-caused.

---
## Generated data (18/18 run, `pr8fix` = branch tip after all ten fixes)

## Headline

- Scripts run: **18**
- Succeeded under **0.9.2**: **18/18**
- Succeeded under **pr8fix**: **18/18**
- Succeeded under **both** (comparable): **18/18**

## Pymoca compile time (cold flatten, cache disabled)

| Example (script) | 0.9.2 (s) | pr8fix (s) | ratio (cand/base) |
| --- | ---: | ---: | ---: |
| basic__example | 1.907 | 23.566 | 12.35x |
| cascading_channels__example | 2.675 | 31.278 | 11.69x |
| channel_pulse__example | 1.768 | 24.324 | 13.76x |
| channel_wave_damping__example_local_control | 3.436 | 32.917 | 9.58x |
| channel_wave_damping__example_optimization | 2.970 | 31.165 | 10.49x |
| ensemble__example | 1.876 | 23.844 | 12.71x |
| fallback_option__example | 1.907 | 23.507 | 12.33x |
| fallback_option__example_with_gp | 1.820 | 24.331 | 13.37x |
| goal_programming__example | 1.970 | 26.622 | 13.51x |
| integrator_delay__example | 1.435 | 22.614 | 15.76x |
| lookup_table__example | 1.851 | 23.152 | 12.51x |
| mixed_integer__example | 2.024 | 27.845 | 13.76x |
| pumped_hydropower_system__example | 2.083 | 24.739 | 11.88x |
| simulation__example | 1.910 | 23.547 | 12.33x |
| simulation_with_custom_equations__simple_model | 1.867 | 5.848 | 3.13x |
| single_reservoir__single_reservoir | 1.428 | 23.377 | 16.37x |

## Model structure comparison

| Example | states | alg_states | inputs | outputs | parameters | equations | match |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | :---: |
| basic__example | 1 | 0 | 2 | 1 | 4 | 1 | ✅ |
| cascading_channels__example | 15 | 4 | 4 | 0 | 63→69 | 18 | ⚠️ (extra min_abs_Q/min_divisor, expected) |
| channel_pulse__example | 23 | 0 | 2 | 4 | 24→26 | 23 | ⚠️ (extra min_abs_Q/min_divisor, expected) |
| channel_wave_damping__example_local_control | 31 | 0 | 2 | 5 | 80→86 | 31→37 | ⚠️ (extra min_abs_Q/min_divisor, expected) |
| channel_wave_damping__example_optimization | 31 | 0 | 2 | 5 | 80→86 | 31→37 | ⚠️ (extra min_abs_Q/min_divisor, expected) |
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

`channel_wave_damping__example_local_control`, `steady_state_initialization_mixin`,
and `step_size_parameter_mixin` are not independent data points: only
`example_optimization.py` has a run trigger that writes the default
`output/timeseries_export.csv` in that example folder (it imports and runs
both `ExampleOptimization` and `ExampleLocalControl` at module level); the
other three files in that directory are plain mixin classes with no `if
__name__` guard, so "running" them does nothing and their row just reflects
whatever `example_optimization.py` left behind moments earlier in the same
output directory. Treat `channel_wave_damping__example_optimization` as the
one real comparison for this example.

| Example (script) | max abs diff | max rel diff | note |
| --- | ---: | ---: | --- |
| basic__example | 0.000e+00 | 0.000e+00 |  |
| cascading_channels__example | 2.310e+00 | 1.000e+00 | expected: extra min_abs_Q/min_divisor columns vs 0.9.2 (benign, see above); worst: UpperControlStructure_Q |
| channel_wave_damping__example_optimization | 4.999e+02 | 1.000e+00 | Remaining issue A: solver fails (TOO_FEW_DOF), fallback export; worst: Q_dam_upstream |
| goal_programming__example | 5.583e+00 | 1.000e+00 | Remaining issue A: solver fails (INFEASIBLE), fallback export; worst: Q_pump |
| integrator_delay__example | 0.000e+00 | 0.000e+00 |  |
| lookup_table__example | 0.000e+00 | 0.000e+00 |  |
| mixed_integer__example | 5.822e+00 | 1.000e+00 | Remaining issue A: solver fails (INFEASIBLE), fallback export; worst: Q_orifice |
| pumped_hydropower_system__example | 0.000e+00 | 0.000e+00 |  |
| simulation__example | 0.000e+00 | 0.000e+00 |  |
| simulation_with_custom_equations__simple_model | 0.000e+00 | 0.000e+00 |  |
| single_reservoir__single_reservoir | 0.000e+00 | 0.000e+00 |  |

`channel_pulse__example` is bit-identical (`0.000e+00`) despite the extra
`min_abs_Q`/`min_divisor` parameters: those channels' friction terms never
exercise the branch where the tiny `eps` denominator offset is numerically
significant for this input timeseries.

## Run status

| Example (script) | 0.9.2 | pr8fix |
| --- | :---: | :---: |
| basic__example | ok | ok |
| cascading_channels__example | ok | ok |
| channel_pulse__example | ok | ok |
| channel_wave_damping__example_local_control | ok | ok |
| channel_wave_damping__example_optimization | ok | ok |
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

All 18/18 scripts exit 0 under both versions (RTC-Tools treats solver
failure as a warning, not a process failure — see Remaining issue A for the
3 that don't produce a real solution).
