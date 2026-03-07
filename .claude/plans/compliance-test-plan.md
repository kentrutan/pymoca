# Automate ModelicaCompliance Tests for Name Lookup, Instantiation, and Flattening

## Context

The pymoca project has a `Modelica-Compliance` git submodule (`test/libraries/Modelica-Compliance`) containing standardized Modelica language compliance test models. Currently, compliance tests are hand-written in `test/name_lookup_test.py` (~40 methods) and `test/parse_test.py` (import flattening tests at line 1480+). These hand-written tests cover only a subset of available compliance models and require manual effort to add new ones.

The goal is to create automated test infrastructure that discovers `.mo` files from the compliance suite, reads their `shouldPass` annotation, and runs them through three levels of pymoca processing (name lookup, instantiation, flattening). The infrastructure should be scalable to eventually cover all ModelicaCompliance categories beyond the initial `Scoping/NameLookup` scope.

### Branch: `flattening-per-spec`

This plan targets the `flattening-per-spec` branch, where `tree.py` has been split into a `tree/` package with submodules:
- `tree/_base.py` — Exceptions (`NameLookupError`, `InstantiationError`, etc.)
- `tree/_name_lookup.py` — `find_name()` (MLS Chapter 5)
- `tree/_instantiation.py` — `instantiate()` (MLS 5.6.1)
- `tree/_flattening.py` — `flatten_instance()` (MLS 5.6.2, new path)
- `tree/_legacy.py` — `flatten()`, `flatten_class()` (old path, preserved for backward compat)
- `tree/__init__.py` — Re-exports all public symbols

The new flattening pipeline is: `instantiate()` → `flatten_instance()`. The legacy `flatten()` is still available. Compliance tests should exercise the new path primarily, with fallback to legacy where the new path has stubs.

## Files to Create

### 1. `test/conftest.py` — Shared discovery infrastructure

Contains:

- **`COMPLIANCE_DIR`** / **`COMPLIANCE_AVAILABLE`** — Path constants and a boolean check for whether the submodule is initialized (checks `Icons.mo` exists)
- **`parse_should_pass(mo_file_path) -> bool`** — Regex-based extraction of `shouldPass = true|false` from the `__ModelicaAssociation(TestCase(...))` annotation in `.mo` files
- **`discover_compliance_files(subdirectory) -> list[tuple[str, str, bool]]`** — Walks a subdirectory under `COMPLIANCE_DIR`, finds all `.mo` files (excluding `package.mo`, `package.order`), returns `(test_id, abs_path, should_pass)` tuples
- **`mo_path_to_model_name(mo_file_path) -> str`** — Converts file path to fully-qualified Modelica class name (e.g., `ModelicaCompliance.Scoping.NameLookup.Simple.Encapsulation`)
- **`load_compliance_model(mo_file_path) -> ast.Tree`** — Parses `Icons.mo` + the target `.mo` file and returns merged tree (mirrors existing `parse_lookup_file` pattern from `name_lookup_test.py:29-36`)
- **`KNOWN_FAILURES`** — Dict mapping `(test_level, model_name)` to xfail reason strings. Populated after initial test run
- **`build_params(test_level, categories)`** — Builds `pytest.param` list with conditional `xfail` marks from the registry
- **`pytest_configure(config)`** — Registers custom markers: `compliance`, `name_lookup`, `instantiation`, `flattening`

### 2. `test/compliance_test.py` — Parametrized test functions

Contains:

- **Module-level skip**: `pytestmark = pytest.mark.skipif(not COMPLIANCE_AVAILABLE, ...)`
- **Category definitions**: Initially `Scoping/NameLookup/{Simple,Composite,Global,Imports}`
- **Three parametrized test functions**:

**`test_compliance_name_lookup(mo_path, model_name, should_pass)`**
- Parses the `.mo` file via `load_compliance_model()`
- Uses `find_name(relative_name, scope)` to locate the model class starting from `ast_tree.classes["ModelicaCompliance"]`
- `shouldPass=true`: asserts result is not None
- `shouldPass=false`: expects `NameLookupError` or None result

**`test_compliance_instantiate(mo_path, model_name, should_pass)`**
- Calls `tree.instantiate(model_name, ast_tree)`
- `shouldPass=true`: asserts instance is not None
- `shouldPass=false`: expects exception (`NameLookupError`, `InstantiationError`, `ClassNotFoundError`)

**`test_compliance_flatten(mo_path, model_name, should_pass)`**
- Uses the new-path pipeline: `tree.instantiate(model_name, ast_tree)` → `tree.flatten_instance(instance)`
- Falls back to legacy `tree.flatten(ast_tree, ast.ComponentRef.from_string(model_name))` if new path raises `NotImplementedError`
- `shouldPass=true`: asserts flat result is not None and contains expected symbols
- `shouldPass=false`: expects exception

## Files NOT Modified

- `test/name_lookup_test.py` — Existing hand-written tests remain untouched (they have detailed value-checking assertions beyond what automated tests provide)
- `test/parse_test.py` — Existing flattening tests remain untouched
- `.github/workflows/ci.yml` — Already checks out submodules (line 37: `submodules: true`)

## Key Reuse Points

- Pattern from `test/name_lookup_test.py:29-36` — `parse_lookup_file()` merging Icons.mo + target
- Pattern from `test/parse_test.py:1481-1486` — `parse_imports_file()` for flattening
- API: `pymoca.tree.find_name()` (tree/_name_lookup.py:17), `pymoca.tree.instantiate()` (tree/_instantiation.py:46), `pymoca.tree.flatten()` (tree/_legacy.py:993), `pymoca.tree.flatten_instance()` (tree/_flattening.py:24)
- Exceptions: `tree.NameLookupError` (tree/_base.py:41), `tree.InstantiationError` (tree/_base.py:47), `ast.ClassNotFoundError` (ast.py:16)
- All public symbols re-exported from `pymoca.tree.__init__` for backward compatibility

## Scalability Design

Adding new compliance categories requires only extending the category list:
```python
# Future additions:
INHERITANCE_CATEGORIES = ["Inheritance/Basic", "Inheritance/Extends", ...]
MODIFICATION_CATEGORIES = ["Modification/...", ...]
```

Each category uses the same discovery → parametrize → test pipeline.

## Phase 1 — Structural compliance tests (this implementation)

### Implementation Steps

1. Initialize the Modelica-Compliance submodule locally (`git submodule update --init test/libraries/Modelica-Compliance`)
2. Create `test/conftest.py` with discovery infrastructure
3. Create `test/compliance_test.py` with three parametrized test functions
4. Run tests to identify failures: `pytest test/compliance_test.py -v`
5. Populate `KNOWN_FAILURES` with xfail entries for genuine pymoca limitations
6. Verify existing tests still pass: `pytest test/name_lookup_test.py test/parse_test.py -v`

### Verification

1. `pytest test/compliance_test.py -v` — All compliance tests should pass or be marked xfail
2. `pytest test/compliance_test.py -m name_lookup -v` — Run only name lookup level
3. `pytest test/ -v` — Full suite including existing tests still passes
4. Without submodule: `pytest test/compliance_test.py -v` — All tests skip gracefully
5. `pytest test/compliance_test.py --co` — Verify test collection shows parametrized test IDs like `test_compliance_name_lookup[Simple/Encapsulation]`

## Phase 2 — Value checking and simulation (future)

Phase 2 adds the ability to automatically verify the assertion values embedded in each compliance `.mo` model (e.g., `assert(b.a.x == 2, ...)`), which the hand-written tests in `name_lookup_test.py` and `parse_test.py` currently check manually. This replaces the many `# TODO: flatten and check x.value` comments in `name_lookup_test.py`.

### Design hooks built into Phase 1

The Phase 1 infrastructure is designed to support Phase 2 without restructuring:

1. **`conftest.py` will include `parse_assertions(mo_file_path) -> list[AssertionInfo]`** — A regex or parser-based function to extract `assert(...)` statements from `.mo` files. Compliance models use two assertion patterns:
   - Direct comparison: `assert(b.a.x == 2, "message")` — common for integer/constant checks
   - Utility comparison: `assert(Util.compareReal(expr, value), "message")` — used for Real-valued checks with tolerance

   Returns a list of `AssertionInfo` namedtuples with fields `(lhs: str, operator: str, rhs: str|float)`. This function is defined in Phase 1 (even if not called by Phase 1 tests) to establish the interface.

   Additionally, **`parse_experiment(mo_file_path) -> dict`** extracts `experiment(StopTime=..., ...)` annotation parameters, returning a dict like `{"StopTime": 0.01}`. Needed by Phase 2b for simulation setup.

2. **`KNOWN_FAILURES` dict already supports a `"value_check"` test level** — The xfail registry uses `(test_level, model_name)` keys, so adding `("value_check", "ModelicaCompliance.Scoping....")` entries is straightforward.

3. **`build_params` already accepts a `test_level` argument** — Phase 2 test functions call `build_params("value_check", ...)` or `build_params("simulate", ...)`.

### Phase 2a — Expression evaluation without simulation (implemented)

Adds a `test_compliance_value_check` parametrized test function that:

1. Flattens the model via `tree.instantiate()` → `tree.flatten_instance()`
2. Extracts the assertion conditions from the `.mo` file using `parse_assertions()`
3. For each assertion like `assert(b.a.x == 2, ...)`:
   - Looks up the symbol `b.a.x` in `flat.symbols`
   - Checks if the value resolved to a numeric literal (int/float)
   - Compares against the expected value (exact for `==`, tolerance for `Util.compareReal`)
4. Skips assertions where the value is not yet resolved to a literal (e.g. InstanceSymbol references)

**Implementation approach:**
- Direct literal checking — no expression evaluator needed for the current compliance models
- Two regex patterns match `assert(var == value, "msg")` and `assert(Util.compareReal(var, value), "msg")`
- `AssertionInfo` namedtuple with `(variable, expected, approx)` fields
- Models with no assertions or shouldPass=false are skipped

Results: 12 passed, 40 skipped, 6 xfailed across NameLookup categories.
8 of 18 assertions checked (direct literal values only).

### Phase 2b — InstanceSymbol resolver for enhanced value checking (implemented)

Enhances `_resolve_symbol_value()` to follow InstanceSymbol reference chains through
`flat.symbols` to find resolved numeric values. Three resolution strategies:

1. **Prefix-based flat lookup**: `b.y` → ref `x` → try `b.x` in flat.symbols
2. **ast_ref.class_modification fallback**: For package constants not in flat.symbols,
   extracts value from the original Symbol's `class_modification` (e.g., `constant Real x = 5.1`)

Also adds `_extract_value_from_class_mod()` helper and a skip when 0 assertions resolve.

**Results:** 12 passed, 40 skipped, 6 xfailed. 16 of 18 assertions now checked (up from 8).
Remaining unresolved:
- `ImplicitShadowingFor.y` — requires equation solving at runtime
- `NestedCompLookup.y` — `flatten_instance()` stores ref as `C.B.A.x` (class names) instead
  of `c.b.a.x` (component names); flattening bug to fix separately

### Phase 2c — Simulation-based verification (blocked)

Adds a `test_compliance_simulate` parametrized test function that:

1. Flattens the model and generates backend code (CasADi or sympy backend)
2. Runs the simulation with the experiment parameters from the `.mo` annotation (`StopTime`, etc.)
3. Evaluates the `assert()` conditions at the final time step
4. Verifies all assertions pass

This covers dynamic models where assertion values depend on simulation results, not just compile-time constants.

**Status:** Blocked until the new flattening pipeline (`flatten_instance()`) is plugged into
the CasADi/sympy backends. Currently only the legacy `flatten_class()` path feeds into backends.

**Backend options:**
- **CasADi backend** (`pymoca.backends.casadi`): The primary simulation backend, already well-tested
- **Sympy backend** (`pymoca.backends.sympy`): Alternative for pure symbolic evaluation

Phase 2c tests would use additional markers (`pytest.mark.simulate`) and could be excluded from fast CI runs via `-m "not simulate"`.

### Phase 2 test markers

```python
# Added in Phase 2:
config.addinivalue_line("markers", "value_check: Compile-time value checking test")
config.addinivalue_line("markers", "simulate: Simulation-based verification test")
```
