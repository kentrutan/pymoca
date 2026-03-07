#!/usr/bin/env python
"""
Tools for tree walking and visiting etc.

This package re-exports all public symbols for backward compatibility.
The implementation is split across submodules:

  _base.py           — TreeListener, TreeWalker, exceptions
  _name_lookup.py    — find_name and helpers (MLS 5.3, other details throughout Chapter 5)
  _instantiation.py  — instantiate, InstanceTree (MLS 5.6.1)
  _flattening.py     — flatten_instance and helpers (MLS 5.6.2)
  _legacy.py         — old flatten path (flatten_class, flatten, etc.)
"""

from ._base import (  # noqa: F401
    InstantiationError,
    ModelicaError,
    ModelicaSemanticError,
    ModificationTargetNotFound,
    NameLookupError,
    TreeListener,
    TreeWalker,
)
from ._flattening import (  # noqa: F401
    flatten_instance,
)
from ._instantiation import (  # noqa: F401
    InstanceTree,
    instantiate,
)
from ._legacy import (  # noqa: F401
    add_state_value_equations,
    add_variable_value_statements,
    annotate_states,
    apply_constant_references,
    apply_symbol_modifications,
    build_instance_tree,
    expand_connectors,
    extends_builtin,
    flatten,
    flatten_class,
    flatten_component_refs,
    flatten_extends,
    flatten_symbols,
    fully_scope_function_calls,
    modify_symbol,
)
from ._name_lookup import (  # noqa: F401
    find_name,
)
