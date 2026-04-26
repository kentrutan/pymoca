# Pymoca Architecture (flattening-per-spec)

Two trees coexist: lexical (`ast.Tree`/`ast.Class`, `parent` pointers) and instance (`InstanceTree`/`InstanceClass`, `parent_instance` pointers, `ast_ref` back-pointer to the lexical tree).

`InstanceClass(InstanceElement, Class)` via MRO. `InstanceSymbol(InstanceElement, Symbol)` likewise.

## Package Layout

`src/pymoca/tree/` is split into five submodules:

| Module              | Contents                                               |
| ------------------- | ------------------------------------------------------ |
| `_base.py`          | `TreeListener`, `TreeWalker`, exception hierarchy      |
| `_name_lookup.py`   | `find_name` and helpers (MLS Chapter 5)                |
| `_instantiation.py` | `instantiate`, `InstanceTree` (MLS 5.6.1)              |
| `_flattening.py`    | `flatten_instance`, `flatten_to_tree` (MLS 5.6.2)      |
| `_legacy.py`        | Legacy path (`flatten_class`, `flatten_extends`, etc.) |

`__init__.py` re-exports all public symbols for backward compatibility and defines `USE_NEW_FLATTENING = True` (default), which routes `flatten()` through `flatten_to_tree()` instead of the legacy path.

## Name Lookup (MLS Chapter 5)

Entry: `find_name(name, scope)` → `_find_name()` → `_find_simple_name()` / `_find_rest_of_name()`

- **Simple name** (MLS 5.3.1): local → inherited → imported → parent scope (stop at `encapsulated`)
- **Composite name** (MLS 5.3.2): lookup first part, then search in found element
- Falls back from instance tree to class tree via `ast_ref` when not found
- Cycle detection via a shared mutable `RecursionGuard` dataclass (`_base.py`); per-call configuration via the frozen `LookupOptions` dataclass. The guard is mutable so every recursive call sees mutations made by its callees; `LookupOptions` is frozen so internal call sites can `replace()` for variants without aliasing the caller.
- Global name lookup `.A.B.C` deferred (see Known WIP)

## Instantiation (MLS 5.6.1)

Entry: `instantiate(class_name, class_tree)` → `_instantiate_class()`

Steps per spec 5.6.1:
1. Find lexical parent instance (`_get_lexical_parent_instance`)
2. `_instantiate_partially()` — create InstanceClass, merge modifiers
3. `_apply_redeclares()` — validate replaceable/final, lookup in modification scope
4. Partially instantiate local classes, symbols, extends
5. Copy equations/algorithms/annotations
6. `_instantiate_symbol()` — lookup type, recurse
7. `_instantiate_extends_list()` — two-pass: partial instantiate all → check rules → full instantiate

`InstanceElement` carries an `InstantiationState` (`NONE`/`PARTIAL`/`FULL`) IntEnum so guard conditions can be expressed as ordered comparisons (e.g. `state >= PARTIAL`); the older `partially_instantiated`/`fully_instantiated` booleans and the `partially: bool` parameter are gone.

## Flattening (MLS 5.6.2)

Entry: `flatten_instance(instance)` → `_flatten_instance()`
Adapter: `flatten_to_tree(root, class_name)` → `ast.Tree` (for backend compatibility)

`_flatten_instance()` implements spec 5.6.2 step by step:
- **1.1** Insert symbols with dot-separated flat names; strip `input`/`output` prefixes from nested symbols
- **1.2** `_evaluate_conditional_declarations()` — TODO stub
- **1.3** `_resolve_dimensions()` — resolves parametric array dimensions; propagates outer symbol dimensions to inner flat symbols
- **1.4–1.5** `_resolve_modifications()` — resolve value/attribute modifications for simple types; records TODO
- **1.6** Recurse into non-simple type symbols; propagate outer symbol's prefixes (e.g. `parameter`) and dimensions to the leaf builtin symbol
- **1.7** `_collect_and_resolve_equations()` — resolve `ComponentRef`s to flat instance names; discover function calls
- **1.8** Recurse into unnamed extends instances (their symbols/equations are appended after the locals already inserted in 1.1–1.7)
- **1.9** `_check_all_references_valid()` — TODO stub

After `_flatten_instance()` returns, `flatten_instance()` runs in order:
- **Deferred ref-name fixup** — `_flatten_value_ref_names()` rewrites `InstanceSymbol` values whose `.name` is still the class path (e.g. `Pkg.A.x`) to the correct flat name (e.g. `x`). This happens when a value modification references an inherited symbol: `_resolve_name` falls back to the class tree and returns an InstanceSymbol with the full class path because the extends instance is not yet registered in `flat_class.symbols`. The fix runs after the entire extends chain has been walked (when the flat namespace is complete), matching by `ast_ref.full_name`.
- **1.4 (2nd pass)** `_generate_value_equations()` — converts resolved `.value` on non-parameter/non-constant symbols into equations, clears symbol `.value` to sentinel. RHS `ComponentRef`s still in source scope are resolved to flat names via `_EquationRefResolver`.
- **1.9** `_check_all_references_valid()` — TODO stub
- **3** `_process_transitions()` — TODO stub
- **2** `_generate_connect_equations()` — connect expansion; uses `_flatten_connect_ref`, `_is_inner_connector`

`_flatten_discovered_functions()` recursively flattens functions found during step 1.7 (E).

`flatten_to_tree()` converts the `InstanceClass` result back to `ast.Tree` via `_instance_to_ast_class` / `_instance_to_ast_symbol` / `_add_connector_symbols`.

## Legacy Path

`flatten()`, `flatten_class()`, `flatten_extends()`, `build_instance_tree()`, `flatten_symbols()`
— preserved in `_legacy.py` for backward compatibility. `Class.find_class()` delegates to `tree.find_name()`
via `Class.use_find_name(True)`.

## Design Decisions

### Scope Tracking

The `scope` field on `ClassModificationArgument` and `ExtendsClause` records where modification
values should be looked up. Parser sets `scope = current_class`. For short class definitions
(`class A = B(mod)`), scope is the *enclosing* class. During instantiation,
`_update_class_modification_scopes()` converts AST Class → InstanceClass. During flattening,
modifications are resolved in `arg.scope`.

Two roles that previously coincided are kept separate (MLS 4.5.1, 5.6 step B):

- **Lookup scope** — which class to search. Stays at the syntactic site of the modification (`arg.scope`); that is how the user wrote it.
- **Flat-naming root** — which class to name the resolved element relative to. Becomes the model being flattened.

When a modification ComponentRef resolves to an inherited element, the resolved element is **cloned and reparented** under the flat root for path-name purposes but **not registered** in the flat class's symbol table. Registration still happens through the element's own definition site during the normal flatten walk; this avoids double-registration. `_resolve_name` walks the full scope-to-`InstanceClass` chain and uses `parent_instance` (instance hierarchy), not `parent` (lexical).

For Expression-valued modifications, constant folding runs first; when it gives up because parameter values have not yet been propagated, a fallback ComponentRef-rewriting walker applies the same global-path replacement so kept Expressions emerge with globally rooted names (e.g. `e.H_b` rather than `H_b`).

### Extends Lookup: `search_inherited=False/True`

Per MLS 5.6.1 and ModelicaCompliance `InheritedBaseClass` test, names in extends-clauses
may NOT be inherited. The `search_inherited=False` parameter prevents looking in
inherited elements for simple name lookup such as `A` in `A.B.C`. Once the first name is
found, the rest of the composite name `B.C` should use normal lookup including inherited
elements with `search_inherited=True`.

`_check_extends_rules` enforces the rule on **every** identifier of the as-written tuple
(threaded through `ExtendsClause.component.to_tuple()`), not just the leaf — so a class
that inherits a symbol `B` from one base and then writes `extends B.C` from another base
is caught. The check runs at the boundary between partial and full extends-instantiation
passes; earlier the inherited-name table is not yet populated, later full instantiation has
already failed in a less informative way.

### Replaceable/Redeclare (MLS 7.3)

- Only `replaceable` elements may be redeclared
- `redeclare` without `replaceable` is an error (stricter than OpenModelica)
- Plug-compatibility checks (MLS Ch.6) deferred
- Builtin-typed redeclares (`redeclare Real x = 4.0`) assign the redeclare class directly as the symbol type rather than mutating the resolved builtin's `extends`. Builtin types are re-instantiated from `ast_ref` on every reference, so any mutation to the resolved type's state would be wiped on the next instantiation; carrying the modifications on the redeclare class itself replays them on every fresh instance.

### Extends Processing

Two-pass in `_instantiate_extends_list()`:
1. Partial: find extends class, merge mods, create unnamed InstanceClass
2. Check rules: no name collisions, no mixing builtin/non-builtin extends
3. Full: recursively instantiate each extends

### Symbol/Equation Ordering

`_flatten_instance()` emits local symbols and equations first, then recurses into
unnamed extends instances, so inherited entries are appended after local ones in
the flat output. Applies to symbols, equations, and initial_equations.

## Known WIP / Stubs

- Global name lookup (MLS 5.3.3)
- `_evaluate_conditional_declarations` (MLS 5.6.2 step 1.2)
- `_process_transitions` (MLS 5.6.2 step 3)
- `_check_all_references_valid` (MLS 5.6.2 step 1.9)
- Record modifications in `_resolve_modifications`
- `ExpressionEvaluator` — partial implementation; uses an operator-dispatch table (no `eval()`)
- Instance→AST fallback in `_find_name()` not strictly spec-compliant but backward compatible
- Built-in functions/operators not yet added to `InstanceTree`
- Iteration variables in name lookup (`for i = i:i+3`)
- Type compatibility / constraining-type / prefix preservation for redeclares (`FIXME` markers in `_instantiation.py`)

## References

- PR #307 has extensive discussion (71 PR + 218 code review comments).
- ModelicaCompliance = reference for testing correctness.
