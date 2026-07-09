# pymoca

A Modelica to computer algebra system (CAS) translator written in Python.

Pymoca can be used in applications that need to translate [Modelica](https://modelica.org)
mathematical models into other forms. Pymoca can "flatten" a model containing a connected set
of components defined by object-oriented Modelica classes into a set of variables and
simultaneous equations that are easier to further process for analysis or simulation. Pymoca
can translate Modelica to [CasADi](https://web.casadi.org), [SymPy](https://www.sympy.org), and
[ModelicaXML](https://github.com/modelica-association/ModelicaXML), but most development and
usage has been with CasADi.

See the project [README](https://github.com/pymoca/pymoca#readme) for installation and usage
instructions.

## Examples

The "[Pymoca new flattening and related features — a guided tour](pymoca_011_vs_new.ipynb)"
Jupyter notebook compares output between the current version and pymoca 0.11
on a set of examples showing some of the new features.

To run it live with a Jupyter kernel from the top-level of the repo:
```bash
pip install "pymoca[examples]"
jupyter lab doc/pymoca_011_vs_new.ipynb
```

## Migrating from pymoca 0.11

API changes a 0.11 user is most likely to hit:

- `tree.flatten(root, class_name)` still returns an `ast.Tree` with the flat class
  keyed by its full dotted name. Flat symbols are now ordered local-before-inherited
  (see [architecture](architecture.md) "Symbol/Equation Ordering and Storage").
- `tree.flatten_class(root, class_name)` is a **different function** than the 0.11
  helper of the same name: it instantiates and flattens `class_name` and returns the
  flat `tree.InstanceClass`, the new pipeline's native result.
- The instance types (`InstanceClass`, `InstanceSymbol`, ...) moved from `pymoca.ast`
  to `pymoca.tree`.
- Errors are reported through the `tree.ModelicaError` hierarchy (`NameLookupError`,
  `InstantiationError`, `ModelicaSemanticError`), replacing `ast.ClassNotFoundError`
  and friends. `parser.parse` raises `parser.ModelicaSyntaxError` instead of
  returning `None`.
- Internal flattening helpers such as `tree.expand_connectors`,
  `tree.flatten_component_refs`, and `tree.ComponentRefFlattener` are no longer
  public; the pipeline applies these steps itself. The public surface is now declared
  with `__all__` in `pymoca.ast` and `pymoca.tree`.

## Architecture

See [architecture](architecture.md) for the pymoca flattening architecture and how it
follows the Modelica Language Specification (MLS).
