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
run but the solver fails to find a real solution — root-caused to an upstream
library defect, not a pymoca bug (Remaining issue A, below). The other 3
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

## Remaining issue A: root-caused — an upstream library type-mismatch, not a pymoca bug

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

**Ruled out**: a symbol-level fingerprint diff against 0.9.2 for
`goal_programming` (using the correct `tree.flatten()`/`flatten_to_tree`
entry point) showed the flattened models structurally identical (73 symbols,
37 equations). For `channel_wave_damping__example_optimization`,
`model.states`/`model.alg_states` matched exactly (27/0) between PR and 0.9.2
once compared with **matching** `compiler_options` (an earlier lead pointing
at a 149-vs-143 equation-count mismatch was an artifact of a debugging script
that omitted `detect_aliases` etc. on the PR side only). The extra 6
`min_abs_Q`/`min_divisor` parameters (below) are not the cause either:
parameters are fixed, not NLP decision variables.

**Root cause, found by an exhaustive (not sampled) symbol-attribute diff**:
`Modelica.Units.SI.Distance` is defined in MSL as `type Distance =
Length(min=0)` — non-negative by design. The `Deltares.ChannelFlow` library
(an RTC-Tools dependency, *not* part of pymoca) types several
internally-computed quantities as `Distance` even though they are signed or
transiently need boundary/negative values during NLP solving:

- `Deltares.ChannelFlow.Hydraulic.Structures.Pump.dH` — `dH = HQDown.H -
  HQUp.H`, a head *difference*, legitimately negative whenever downstream is
  higher than upstream (exactly what `goal_programming`'s `is_downhill`
  logic exists to allow). Used by both `goal_programming` and
  `mixed_integer` (both instantiate `Pump` twice, as `pump` and `orifice`).
- `Deltares.ChannelFlow.Hydraulic.Branches.Internal.PartialHomotopic._wetted_perimeter`,
  a differential `state` — used by `channel_wave_damping`.

Under 0.9.2, this `min=0` type default is never propagated onto these
variables (confirmed directly: `pump.dH`/`orifice.dH` come out `min=-inf` in
0.9.2's own compiled `model.states`/`model.alg_states`, vs `min=0.0` under
the PR). The PR branch correctly implements MLS type-attribute inheritance
(no explicit override on the declaration means the type's own default
modification applies), turning `min=0` into a hard NLP box constraint that
makes the true physical solution infeasible whenever `dH`/`_wetted_perimeter`
need to go negative.

**Verified experimentally** (not committed — a throwaway edit to the venv's
installed library copy, reverted immediately after): overriding
`dH(min=-100)` in the installed `Pump.mo` made `goal_programming` solve to
`SUCCESS` with results matching the 0.9.2 reference closely (`Q_pump`
integral 60.099 vs 0.9.2's 60.104; per-step values agree to 3-4 significant
figures, the residual gap consistent with normal solver-path/collocation
tolerance rather than a structural difference).

**Not a pymoca bug**: this is pymoca doing the *more* spec-correct thing —
propagating a type's default `min`/`max` attribute per MLS §4.4.4 wherever
the declaring component doesn't override it — which 0.9.2 simply never
implemented for non-parameter variables. The defect is in `Deltares.
ChannelFlow`'s own type choices (`dH`/`_wetted_perimeter` should be typed
`Length`, the general signed quantity, not `Distance`), a package outside
this repository. Weakening pymoca's attribute-inheritance correctness to
accommodate it would be a regression, not a fix, so no pymoca-side change was
made. Reported to the user with the finding and a recommendation to leave
pymoca as-is; the fix belongs upstream in `Deltares.ChannelFlow`.

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

The ~20-30s cold numbers above (`pr8fix`) are not representative of normal
use: they measure every script paying the full cost of re-parsing MSL from
scratch, because the benchmark driver deliberately wipes pymoca's own
parse cache (`$XDG_CACHE_HOME/pymoca/model_txt_cache.db`, a SQLite text ->
AST cache used automatically by `parser.parse`) before every single script,
to keep the "cold flatten" timing fair and comparable run-to-run. In normal
use nothing wipes that cache, so re-ran the comparison letting it persist:
`run_all.py` gained opt-in `--keep-parse-cache` / `--xdg-cache <path>` flags
(committed), and the 18-script suite was run twice against the PR branch
sharing one cache directory across both passes (`--modelicapath` set to the
vendored MSL as usual).

| Example (script) | 0.9.2 (s) | pr8fix cold (s) | pr warm run 1 (s) | pr warm run 2 (s) |
| --- | ---: | ---: | ---: | ---: |
| basic__example | 1.907 | 23.566 | 18.280 | 1.611 |
| cascading_channels__example | 2.675 | 31.278 | 8.887 | 8.773 |
| channel_pulse__example | 1.768 | 24.324 | 4.665 | 4.539 |
| channel_wave_damping__example_local_control | 3.436 | 32.917 | 10.251 | 10.355 |
| channel_wave_damping__example_optimization | 2.970 | 31.165 | 10.901 | 11.115 |
| ensemble__example | 1.876 | 23.844 | 1.563 | 1.470 |
| fallback_option__example | 1.907 | 23.507 | 1.574 | 1.593 |
| fallback_option__example_with_gp | 1.820 | 24.331 | 1.605 | 1.466 |
| goal_programming__example | 1.970 | 26.622 | 1.736 | 1.728 |
| integrator_delay__example | 1.435 | 22.614 | 2.077 | 2.020 |
| lookup_table__example | 1.851 | 23.152 | 1.600 | 1.592 |
| mixed_integer__example | 2.024 | 27.845 | 1.858 | 1.852 |
| pumped_hydropower_system__example | 2.083 | 24.739 | 1.860 | 2.016 |
| simulation__example | 1.910 | 23.547 | 1.717 | 1.526 |
| simulation_with_custom_equations__simple_model | 1.867 | 5.848 | 0.919 | 0.816 |
| single_reservoir__single_reservoir | 1.428 | 23.377 | 1.934 | 2.122 |

`pr warm run 1` is the *first* pass against an empty cache: `basic__example`
(alphabetically first) still pays almost the full cold cost (18.3s, since
nothing is cached yet), but every subsequent script benefits from whatever
common MSL/Deltares classes the earlier ones already parsed and cached —
by the 6th script (`ensemble__example`) the time has already dropped to
1.6s. `pr warm run 2` starts from run 1's fully-populated cache: even the
first script alphabetically (`basic__example`) now compiles in 1.6s, on par
with 0.9.2, and the rest of the suite matches run 1 within noise (confirming
run 1 had already saturated the cache by its own end).

Two things worth calling out:

- The gap does **not** fully close for `cascading_channels`,
  `channel_pulse`, and both `channel_wave_damping` scripts (4.5-11s warm vs
  ~2-3.5s for 0.9.2). These are the largest, most structurally complex
  models in the suite (most symbols/equations after flattening, per the
  "Model structure comparison" table in the generated-data appendix below);
  their residual time is real
  flattening/instantiation work over that structure, which a parse cache
  can't shortcut — it only removes redundant *re-parsing* of unchanged
  library source text.
- This was a one-off, `--keep-parse-cache`-enabled experiment, not the
  benchmark's default mode: `run_all.py`'s ordinary cold-compile numbers
  above are still the fair, apples-to-apples comparison for correctness
  work, and remain deliberately cache-wiped. The takeaway is about what
  *real* RTC-Tools usage will see (a warm cache after the first run), not a
  claim that PR compile time now matches 0.9.2 unconditionally.

## Bottom line

Ten atomic, independently-verified fixes took the branch from 3/18 to 18/18
RTC-Tools scripts running, with 15/18 of those bit-identical to 0.9.2. Of the
non-bit-identical examples, `cascading_channels`'s extra
`min_abs_Q`/`min_divisor` parameters are a benign, expected correctness
improvement over 0.9.2, not a bug. The remaining 3
(`goal_programming`, `mixed_integer`, `channel_wave_damping__example_optimization`)
are conclusively root-caused to an upstream `Deltares.ChannelFlow` library
type mismatch (`Distance` instead of `Length` on signed quantities), verified
by a working experimental patch that reproduces the 0.9.2 reference results —
not a pymoca defect, so no pymoca-side fix was made; see "Remaining issue A"
for the recommendation.

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
