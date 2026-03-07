# MLS 3.5 Study Notes — Highlighted Sections Relevant to flattening-per-spec

These notes summarize the sections of the Modelica Language Specification v3.5
(`doc/ModelicaSpec35.pdf`) that were highlighted/annotated during study, and their
relevance to the pymoca `flattening-per-spec` branch.

## Chapter 4: Definitions and Class Structure (pp 42-43)

### Element Definitions (4.4)
- **Element** = class-definition | component-clause | extends-clause | import-clause
- An element is either inherited from a base class or local
- Components = "variables" in the Modelica sense (instances of classes)

### Short Class Definitions (4.5.1)
- `class A = B(mod)` — the modification `mod` is evaluated in the **enclosing** scope,
  not inside A. This is critical for the `scope` tracking on `ClassModificationArgument`
  in our parser/instantiation.
- Short class definitions inherit all properties of the referenced class but allow
  modification and constraining-clause.

**Branch relevance**: The parser's `scope` assignment (parser.py ~line 226) and
`_update_class_modification_scopes()` in tree.py directly implement this rule.

---

## Chapter 5: Scoping, Name Lookup, and Flattening (pp 59-70) — HEAVILY ANNOTATED

This is the core chapter for the branch. Nearly every page was highlighted.

### 5.2 Enclosing Classes
- Each class is enclosed in one other class (except the unnamed root)
- `encapsulated` classes do not search parent scopes during lookup

### 5.3.1 Simple Name Lookup
The lookup order (our `_find_simple_name()`):
1. Local elements of the class (including inherited)
2. Imported names (qualified, wildcard, group imports)
3. Enclosing scope (parent class) — stop at `encapsulated`
4. For the top-level unnamed class: predefined types (Real, Integer, Boolean, String)

**Key rule**: A found name must be identical regardless of which path finds it
(local vs inherited vs imported). Ambiguity is an error.

### 5.3.2 Composite Name Lookup
- `A.B.C` — look up `A` via simple name lookup, then find `B` in `A`, then `C` in `A.B`
- When looking into a class for the "rest of name", the class must be temporarily
  flattened (without modifiers) to see inherited elements
- This is why `_find_rest_of_name()` in tree.py creates a temporary partial instance

### 5.3.3 Global Name Lookup
- `.A.B` (leading dot) — look up `A` in the top-level unnamed class only
- Currently a stub in our implementation

### 5.4 Inner/Outer
- `outer` declarations reference the nearest `inner` of the same name in an
  enclosing instance
- Conditional components are ignored for inner/outer matching

### 5.6.1 Instantiation (THE CORE ALGORITHM)
Steps highlighted in the spec:
1. Find the class to instantiate in the lexical tree
2. Create an instance, set up the parent-instance pointer
3. Apply modifications from the declaration/extends
4. For each local class: recursively instantiate
5. For each component: look up its type class, recursively instantiate
6. For each extends-clause: find the base class, merge modifications, instantiate

**Extends lookup rule (5.6.1.4, p66)**: "The classes of extends-clauses are looked
up before and after handling extends-clauses; and it is an error if those lookups
generate different results." Since extends names must be findable *before* inheritance
is processed, inherited elements aren't available yet — this is the basis for
`search_inherited=False` on the first identifier. The `InheritedBaseClass`
ModelicaCompliance test (`shouldPass=false`, `section={"5.6.1"}`) validates this.

For composite extends names (e.g. `extends A.D`), `search_inherited=False` only
restricts the first identifier. Once `A` is found, `D` is looked up inside `A`'s
flattened class using normal lookup including A's own inheritance (per 5.3.2 composite
name lookup rules). The MLS does not explicitly state this — it's an inference from
combining the circularity prevention rule (5.6.1.4) with composite name lookup (5.3.2).
See `redeclare_order_investigation.md` for the full analysis.

**Key detail**: Modifications are NOT applied during the lookup of the base class
name. They are applied after the base class is identified.

### 5.6.2 Flattening
- Flattening = walking the instance tree and collecting all equations, algorithms,
  and declarations with fully-qualified (dot-separated) names
- Connection equations are generated from `connect()` statements
- Conditional declarations (`if` expressions on components) must be evaluated
  to determine which components exist

**Branch relevance**: `tree.py` lines ~2117-2512 implement this. Several stubs
remain (`_evaluate_conditional_declarations`, `_generate_connect_equations`, etc.)

---

## Chapter 7: Inheritance and Modification (pp 70-92)

### 7.1 Inheritance (extends)
- Elements of the base class are added to the derived class "as if they were
  declared locally"
- The order of extends clauses matters for equation ordering
- Multiple inheritance is allowed (multiple extends clauses)
- Name collisions between inherited and local elements are errors
  (except for redeclare)

### 7.2 Modifications
- Modifications are applied in the scope where they are WRITTEN, not where the
  modified class is defined
- `class A = B(x=1)` — the `1` is evaluated in A's enclosing scope
- Merging: outer modifications take precedence over inner (declaration-level)
  modifications. "Outer" = the modifier at the use site; "inner" = the default
  value in the class definition
- `each` prefix distributes a scalar modification across array dimensions

### 7.3 Redeclaration
- Only elements declared `replaceable` can be redeclared
- `redeclare` keyword is required at the modification site
- `final` prevents further modification/redeclaration

### 7.3.2 Constraining Type
- Every replaceable element has a constraining type
- If no `constrainedby` clause: the constraining type = the declared type with
  any modifications applied
- **Highlighted rule**: "The class or type of component shall be a subtype of the
  constraining type" — this is the type compatibility check for redeclarations
- The redeclared element must be a subtype of the constraining type, not
  necessarily of the original element type

**Branch relevance**: `_apply_redeclares()` in tree.py implements these checks.
The `replaceable` and `final` flags on Class/Symbol in ast.py support this.

---

## Chapter 9: Connectors and Connections (p 115)

### 9.2 Connection Set Generation
- **Highlighted**: Connection sets are built from tuples of primitive variables
  (after expanding structured connectors into their primitive components)
- Each `connect(a, b)` merges the connection sets of matching primitive
  variables in `a` and `b`
- Inside vs outside connector distinction matters for sign of flow variables

**Branch relevance**: `_generate_connect_equations` is currently a stub in tree.py.
This section defines the algorithm that needs to be implemented.

---

## Appendix A: Grammar Reference (pp 280-287)

Key grammar productions relevant to the branch:

- `element-redeclaration`: `redeclare [each] [final] (short-class-definition | component-clause1 | element-replaceable)`
- `element-replaceable`: `replaceable (short-class-definition | component-clause1) [constraining-clause]`
- `modification`: `class-modification ["=" expression] | "=" expression | ":=" expression`
- `extends-clause`: `extends type-specifier [class-modification] [annotation-clause]`

The full grammar is in @doc/modelica_grammar.txt

## Appendix B: DAE Representation (pp 288-290)

Flattening produces a DAE system:
- `0 = f_x(v, c)` — continuous-time equations
- `0 = f_z(v, c)` at events — discrete-time equations
- `m := f_m(v, c)` — discrete-variable assignments
- `c := f_c(relation(v))` — condition evaluation

Variables are categorized as: parameters/constants `p`, time `t`, states `x(t)`,
algebraic `y(t)`, discrete-time Real `z(t_e)`, discrete-valued `m(t_e)`, conditions `c(t_e)`.

This is the target representation that pymoca's sympy backend produces (see `runtime.py`).

---

## Summary: Key Spec Rules for the Branch

1. **Scope tracking**: Modifications are evaluated where WRITTEN, not where the
   class is defined (5.6.1, 7.2)
2. **Extends lookup**: Base class looked up WITHOUT searching inherited names (5.6.1.4);
   for composite names, restriction applies only to first identifier (inferred from 5.3.2)
3. **Instantiation order**: Partial instantiate all extends first, then check rules,
   then full instantiate (5.6.1)
4. **Replaceable/redeclare**: Must be subtype of constraining type (7.3.2)
5. **Simple name lookup order**: local -> inherited -> imported -> parent (5.3.1)
6. **Encapsulated**: Stops parent-scope search (5.2, 5.3.1)
7. **Connection sets**: Built from primitive variable tuples, inside/outside matters (9.2)
8. **Flattening**: Walks instance tree, produces flat DAE with dot-separated names (5.6.2, Appendix B)

## Sections NOT Highlighted (informational)

Chapters 1-3 (overview, operators, expressions), Chapter 6 (interface/subtype),
Chapter 8 (equations), Chapters 10-18 (arrays, statements, functions, packages,
overloaded operators, streams, synchronous, state machines, annotations) had
no visible markup. The study focus was clearly on the scoping/instantiation/
flattening pipeline (Ch 4-5, 7) and connection semantics (Ch 9).
