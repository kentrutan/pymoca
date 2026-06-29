#!/usr/bin/env python
"""
Instance representation produced by instantiation (MLS 5.6.1)

These are the data types output by the instantiation stage and consumed by flattening.
They are kept separate from the parser's AST (``ast.py``): a parse-only consumer needs no
knowledge of these. ``InstanceClass``/``InstanceSymbol`` reuse ``ast.Class``/``ast.Symbol``
via MRO, with ``InstanceElement`` carrying the instantiation-time state.
"""

from __future__ import annotations

import copy
from collections import OrderedDict
from enum import IntEnum

from .. import ast


class InstantiationState(IntEnum):
    NONE = 0  # Not yet instantiated
    PARTIAL = 1  # Local classes/symbols/extends instantiated (step 2.1)
    FULL = 2  # All steps done, symbols recursively instantiated


class InstanceElement:
    """
    Base class for instance elements (symbols, classes, and "unnamed" extends classes)

    This is the "partially instantiated element" in spec 3.5 section 5.6.1.4.
    Includes name for lookup and type for redeclares during instantiation.
    We include the latter two items that are also in sub-classes because we
    want to allow use of this stand-alone as a "partial instance" for memory
    efficiency and speed.
    """

    def __init__(
        self,
        ast_ref: ast.Class | ast.Symbol | None = None,
        modification_environment: ast.ClassModification | None = None,
        parent_instance: InstanceClass | None = None,
        instantiation_state: InstantiationState = InstantiationState.NONE,
        **kwargs,
    ):
        """ast_ref is a reference to the AST node where this instance is defined.
        All named keyword arguments optional for backward compatibility."""

        # super().__init__() is only needed if 1st in method resolution order
        super().__init__(**kwargs)

        self.ast_ref = ast_ref

        if modification_environment is not None:
            self.modification_environment = modification_environment
        else:
            self.modification_environment = ast.ClassModification()

        if "name" in kwargs:
            self.name = kwargs["name"]
        elif ast_ref is not None:
            self.name = ast_ref.name
        else:
            self.name = ""  # The default in Symbol

        if "type" in kwargs:
            self.type = kwargs["type"]
        elif ast_ref is not None:
            self.type = ast_ref.type
        else:
            self.type = ast.ComponentRef()  # The default in Symbol

        self.instantiation_state = instantiation_state
        self.parent_instance = parent_instance

    @property
    def full_instance_name(self) -> str:
        """Return fully-qualified instance name of this element"""
        return element_instance_full_name(self)

    def clone(self):
        """Make a copy of self with copy.copy of lists, dicts, and ClassModifications"""
        new_self = copy.copy(self)
        for key, value in new_self.__dict__.items():
            if isinstance(value, (list, dict, ast.ClassModification)):
                new_self.__dict__[key] = copy.copy(value)
            if isinstance(value, ast.ClassModification):
                new_self.__dict__[key].arguments = copy.copy(value.arguments)
        return new_self

    def __repr__(self):
        return f"name={self.name!r}, ast_ref={self.ast_ref!r}, modification_environment={self.modification_environment!r}"


def element_instance_name_tuple(element: InstanceElement) -> tuple[str, ...]:
    """Return fully-qualified instance name of an element as a tuple of names"""
    names = []
    current = element
    while hasattr(current, "parent_instance"):
        assert current.parent_instance is not None
        # Skip unnamed extends instances AND type class instances
        # (type classes are InstanceClass nodes whose parent is an InstanceSymbol;
        # per MLS 5.6.2 the symbol's component name is used, not the class name)
        if current.name and not (
            isinstance(current, InstanceClass)
            and isinstance(current.parent_instance, InstanceSymbol)
        ):
            names.append(current.name)
        current = current.parent_instance
    return tuple(reversed(names))


def element_instance_full_name(element: InstanceElement) -> str:
    """Return fully-qualified instance name of an element"""
    return ".".join(element_instance_name_tuple(element))


class InstanceClass(InstanceElement, ast.Class):
    """
    Class used during instantiation and flattening of the model.
    """

    # Narrow the resolved-element containers; sound since only instantiation populates them,
    # but mutable members are invariant so pyright flags the override.
    symbols: OrderedDict[str, InstanceSymbol]  # pyright: ignore[reportIncompatibleVariableOverride]
    extends: list[  # pyright: ignore[reportIncompatibleVariableOverride]
        ast.ExtendsClause | InstanceClass
    ]
    classes: dict[str, InstanceClass]  # pyright: ignore[reportIncompatibleVariableOverride]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def __repr__(self):
        return f"{type(self).__name__}({super().__repr__()!s})"


class InstanceSymbol(InstanceElement, ast.Symbol):
    """
    Symbol used during instantiation and flattening of the model.
    """

    # Resolved here from the parsed name; flagged only due to mutable-member invariance.
    type: ast.ComponentRef | InstanceClass  # pyright: ignore[reportIncompatibleVariableOverride]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def __repr__(self):
        return f"{type(self).__name__}({super().__repr__()!s})"


class InstanceTree(ast.Tree):
    """The root class of an instance tree

    :param ast_ref: The root of the `ast.Tree` produced by the parser

    The InstanceTree is the unnamed root of the instance tree containing classes
    instantiated from the `ast.Tree` produced by the parser. Built-in types, functions,
    and operators are to be added to the root of the InstanceTree when it is created so
    they can be found during name lookup.
    """

    # Narrow like InstanceClass.classes: sound since _instantiate_builtins replaces
    # the builtin ast.Class entries with instances before __init__ returns.
    classes: dict[str, InstanceClass]  # pyright: ignore[reportIncompatibleVariableOverride]

    def __init__(self, ast_ref: ast.Tree, **kwargs):
        # The Class AST
        self.ast_ref = ast_ref

        super().__init__(**kwargs)
        self._instantiate_builtins()

    def _instantiate_builtins(self):
        """Add instantiated built-in types to the instance tree"""
        # TODO: Add built-in functions and operators
        # Deferred import: _instantiation imports from this module, so importing it at
        # module load would create a cycle.
        from ._instantiation import _instantiate_class

        builtin_instances = OrderedDict()
        for name, class_ in self.classes.items():
            builtin_instances[name] = _instantiate_class(class_, ast.ClassModification(), self)
        self.classes.update(builtin_instances)
