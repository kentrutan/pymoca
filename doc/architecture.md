# Pymoca Architecture (flattening-per-spec)

Two trees coexist: lexical (`ast.Tree`/`ast.Class`, `parent` pointers) and instance (`InstanceTree`/`InstanceClass`, `parent_instance` pointers, `ast_ref` back-pointer).

`InstanceClass(InstanceElement, Class)` via MRO. `InstanceSymbol(InstanceElement, Symbol)` likewise.

## Name Lookup (MLS Chapter 5)

Entry: `find_name(name, scope)` → `_find_name()` → `_find_simple_name()` / `_find_rest_of_name()`

- **Simple name** (MLS 5.3.1): local → inherited → imported → parent scope (stop at `encapsulated`)
- **Composite name** (MLS 5.3.2): lookup first part, then search in found element
- Falls back from instance tree to class tree via `ast_ref` when not found
- Cycle detection via `current_instances` and `current_extends` sets
- Global name lookup `.A.B.C` deferred

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

## Flattening (MLS 5.6.2)

This is WIP! **Check and update as needed**

Entry: `flatten_instance(instance)` → `_flatten_instance()`

- Walks symbols creating dot-separated flat names
- `_resolve_modifications()` evaluates values using stored `scope`
- `_resolve_name()` maps ComponentRefs to flat instance names
- Stubs: `_evaluate_conditional_declarations`, `_resolve_dimensions`,
  `_generate_connect_equations`, `_process_transitions`, `_check_all_references_valid`

## Legacy Path

`flatten()`, `flatten_class()`, `flatten_extends()`, `build_instance_tree()`, `flatten_symbols()`
— preserved for backward compatibility. `Class.find_class()` delegates to `tree.find_name()`
via `Class.use_find_name(True)`.

## Design Decisions

### Scope Tracking

The `scope` field on `ClassModificationArgument` and `ExtendsClause` records where modification
values should be looked up. Parser sets `scope = current_class`. For short class definitions
(`class A = B(mod)`), scope is the *enclosing* class. During instantiation,
`_update_class_modification_scopes()` converts AST Class → InstanceClass. During flattening,
modifications are resolved in `arg.scope`.

### Extends Lookup: `search_inherited=False/True`

Per MLS 5.6.1 and ModelicaCompliance `InheritedBaseClass` test, names in extends-clauses
may NOT be inherited. The `search_inherited=False` parameter prevents looking in
inherited elements for simple name lookup such as `A` in `A.B.C`. Once the first name is
found, the rest of the composite name `B.C` should use normal lookup including inherited
elements with `search_inherited=True`. The latter was changed in March 2026, so needs
additional time before we accept is as ground truth.

### Replaceable/Redeclare (MLS 7.3)

- Only `replaceable` elements may be redeclared
- `redeclare` without `replaceable` is an error (stricter than OpenModelica)
- Plug-compatibility checks (MLS Ch.6) deferred

### Extends Processing

Two-pass in `_instantiate_extends_list()`:
1. Partial: find extends class, merge mods, create unnamed InstanceClass
2. Check rules: no name collisions, no mixing builtin/non-builtin extends
3. Full: recursively instantiate each extends

## Known WIP / Stubs

- Global name lookup (MLS 5.3.3)
- `ExpressionEvaluator` — partial implementation
- Instance→AST fallback in `_find_name()` not strictly spec-compliant
- Dimensions/array handling in name lookup
- `__value` cleanup

## Collaborators

- **@kentrutan** (Kent Rutan): Primary author. Follows MLS spec structure.
- **@jackvreeken** (Tjerk): Co-maintainer/reviewer. Contributed `search_inherited=False` fix.
- Commit style: squash/fixup, rebase before push.
- PR #307 has extensive discussion (71 PR + 218 code review comments).
- ModelicaCompliance = reference for correctness.

