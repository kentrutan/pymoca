#!/usr/bin/env python
"""
Modelica AST definitions
"""

from __future__ import absolute_import, division, print_function, unicode_literals

import copy
import json
import math
import sys
from collections import OrderedDict
from enum import Enum, IntEnum
from pathlib import Path
from typing import List, Optional, Type, Union  # noqa: F401


class ClassNotFoundError(Exception):
    pass


class Visibility(Enum):
    PROTECTED = 1, "protected"
    PUBLIC = 2, "public"

    def __new__(cls, value, name):
        member = object.__new__(cls)
        member._value_ = value
        member.fullname = name
        return member

    def __int__(self):
        return self.value

    def __str__(self):
        return self.fullname

    def __lt__(self, other):
        return self.value < other.value


class InstantiationState(IntEnum):
    NONE = 0  # Not yet instantiated
    PARTIAL = 1  # Local classes/symbols/extends instantiated (step 2.1)
    FULL = 2  # All steps done, symbols recursively instantiated


nan = float("nan")

"""
AST Node Type Hierarchy

Root Class
    Class
        Equation
            ComponentRef
            Expression
            Primary
        IfEquation
            Expression
            Equation
        ForEquation
            Expression
            Equation
        ConnectClause
            ComponentRef
        Symbol
"""


class Node:
    def __init__(self, **kwargs):
        self.set_args(**kwargs)

    def set_args(self, **kwargs):
        for key in kwargs.keys():
            if key not in self.__dict__.keys():
                raise KeyError("{:s} not valid arg".format(key))
            self.__dict__[key] = kwargs[key]

    def __repr__(self):
        return "{!r}".format(self.__dict__)

    def __str__(self):
        d = self.to_json(self)
        d["_type"] = self.__class__.__name__
        return json.dumps(d, indent=2, sort_keys=True)

    @classmethod
    def to_json(cls, var):
        def guard(var):
            if isinstance(var, Path):
                return var.resolve().as_uri()
            return var

        if isinstance(var, list):
            res = [cls.to_json(guard(item)) for item in var]
        elif isinstance(var, dict):
            res = {key: cls.to_json(guard(var[key])) for key in var.keys()}
        elif isinstance(var, Node):
            # Avoid infinite recursion by not handling attributes that may go
            # back up in the tree again.
            res = {
                key: cls.to_json(guard(var.__dict__[key]))
                for key in var.__dict__.keys()
                if key not in ("parent", "parent_instance", "scope", "__deepcopy__")
            }
        elif isinstance(var, Visibility):
            res = str(var)
        else:
            res = var
        return res


class Primary(Node):
    def __init__(self, **kwargs):
        self.value = None  # type: Optional[Union[bool, float, int, str]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(value={!r})".format(type(self).__name__, self.value)

    def __str__(self):
        return "{} value {}".format(type(self).__name__, self.value)


class Array(Node):
    def __init__(self, **kwargs):
        self.values = []  # type: List[Union[Expression, Primary, ComponentRef, Array]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(values={!r})".format(type(self).__name__, self.values)

    def __str__(self):
        return "{} {}".format(type(self).__name__, self.values)


class Slice(Node):
    def __init__(self, **kwargs):
        self.start = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef]
        self.stop = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef]
        self.step = Primary(value=1)  # type: Union[Expression, Primary, ComponentRef]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(start={!r}, stop={!r}, step={!r})".format(
            type(self).__name__, self.start, self.stop, self.step
        )

    def __str__(self):
        return "{} start: {}, stop: {}, step: {}".format(
            type(self).__name__, self.start, self.stop, self.step
        )


class ComponentRef(Node):
    def __init__(self, **kwargs):
        self.name = ""  # type: str
        self.indices = [
            [None]
        ]  # type: List[List[Optional[Union[Expression, Slice, Primary, ComponentRef]]]]
        self.child = []  # type: List[ComponentRef]
        super().__init__(**kwargs)

    def __repr__(self) -> str:
        # TODO: indices
        if self.child:
            return "{!r}{!r}".format(self.name, self.child)
        else:
            return "{!r}".format(self.name)

    def __str__(self) -> str:
        return ".".join(self.to_tuple())

    def to_tuple(self) -> tuple:
        """
        Convert the nested component reference to flat tuple of names, which is
        hashable and can therefore be used as dictionary key. Note that this
        function ignores any array indices in the component reference.
        :return: flattened tuple of c's names
        """

        if self.child:
            return (self.name,) + self.child[0].to_tuple()
        else:
            return (self.name,)

    @classmethod
    def from_tuple(cls, components: Union[tuple, list]) -> "ComponentRef":
        """
        Convert the tuple pointing to a component to
        a component reference.
        :param components: tuple of components name
        :return: ComponentRef
        """

        component_ref = ComponentRef(name=components[0], child=[])
        c = component_ref
        for component in components[1:]:
            c.child.append(ComponentRef(name=component, child=[]))
            c = c.child[0]
        return component_ref

    @classmethod
    def from_string(cls, s: str) -> "ComponentRef":
        """
        Convert the string pointing to a component using dot notation to
        a component reference.
        :param s: string pointing to component using dot notation
        :return: ComponentRef
        """

        components = s.split(".")
        return cls.from_tuple(components)

    def concatenate(self, arg: "ComponentRef") -> "ComponentRef":
        """
        Helper function to append two component references to eachother, e.g.
        a "within" component ref and an "object type" component ref.
        :return: New component reference, with other appended to self.
        """

        a = copy.deepcopy(self)
        n = a
        b = arg
        while n.child:
            n = n.child[0]
        b = copy.deepcopy(b)  # Not strictly necessary
        n.child = [b]
        return a


class Expression(Node):
    def __init__(self, **kwargs):
        self.operator = None  # type: Optional[Union[str, ComponentRef]]
        self.operands = (
            []
        )  # type: List[Union[Expression, Primary, ComponentRef, Array, IfExpression]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(operator={!r}, operands={!r})".format(
            type(self).__name__, self.operator, self.operands
        )


class IfExpression(Node):
    def __init__(self, **kwargs):
        self.conditions = (
            []
        )  # type: List[Union[Expression, Primary, ComponentRef, Array, IfExpression]]
        self.expressions = (
            []
        )  # type: List[Union[Expression, Primary, ComponentRef, Array, IfExpression]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(conditions={!r}, expressions={!r})".format(
            type(self).__name__, self.conditions, self.expressions
        )


class Equation(Node):
    def __init__(self, **kwargs):
        self.left = (
            None
        )  # type: Union[Expression, Primary, ComponentRef, List[Union[Expression, Primary, ComponentRef]]]
        self.right = (
            None
        )  # type: Union[Expression, Primary, ComponentRef, List[Union[Expression, Primary, ComponentRef]]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(left={!r}, right={!r})".format(type(self).__name__, self.left, self.right)


class IfEquation(Node):
    def __init__(self, **kwargs):
        self.conditions = []  # type: List[Union[Expression, Primary, ComponentRef]]
        self.blocks = (
            []
        )  # type: List[List[Union[Expression, ForEquation, ConnectClause, IfEquation]]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(conditions={!r}, blocks={!r})".format(
            type(self).__name__, self.conditions, self.blocks
        )


class WhenEquation(Node):
    def __init__(self, **kwargs):
        self.conditions = []  # type: List[Union[Expression, Primary, ComponentRef]]
        self.blocks = (
            []
        )  # type: List[List[Union[Expression, ForEquation, ConnectClause, IfEquation]]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(conditions={!r}, blocks={!r})".format(
            type(self).__name__, self.conditions, self.blocks
        )


class ForIndex(Node):
    def __init__(self, **kwargs):
        self.name = ""  # type: str
        self.expression = None  # type: Optional[Union[Expression, Primary, Slice]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(name={!r}, expression={!r})".format(
            type(self).__name__, self.name, self.expression
        )


class ForEquation(Node):
    def __init__(self, **kwargs):
        self.indices = []  # type: List[ForIndex]
        self.equations = []  # type: List[Union[Equation, ForEquation, ConnectClause]]
        self.comment = None  # type: Optional[str]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(indices={!r}, equations={!r})".format(
            type(self).__name__, self.indices, self.equations
        )


class ConnectClause(Node):
    def __init__(self, **kwargs):
        self.left = ComponentRef()  # type: ComponentRef
        self.right = ComponentRef()  # type: ComponentRef
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(left={!r}, right={!r})".format(type(self).__name__, self.left, self.right)


class AssignmentStatement(Node):
    def __init__(self, **kwargs):
        self.left = []  # type: List[ComponentRef]
        self.right = None  # type: Optional[Union[Expression, IfExpression, Primary, ComponentRef]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(left={!r}, right={!r})".format(type(self).__name__, self.left, self.right)


class IfStatement(Node):
    def __init__(self, **kwargs):
        self.conditions = []  # type: List[Union[Expression, Primary, ComponentRef]]
        self.blocks = []  # type: List[List[Union[AssignmentStatement, IfStatement, ForStatement]]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(conditions={!r}, blocks={!r})".format(
            type(self).__name__, self.conditions, self.blocks
        )


class WhenStatement(Node):
    def __init__(self, **kwargs):
        self.conditions = []  # type: List[Union[Expression, Primary, ComponentRef]]
        self.blocks = []  # type: List[List[Union[AssignmentStatement, IfStatement, ForStatement]]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(conditions={!r}, blocks={!r})".format(
            type(self).__name__, self.conditions, self.blocks
        )


class ForStatement(Node):
    def __init__(self, **kwargs):
        self.indices = []  # type: List[ForIndex]
        self.statements = []  # type: List[Union[AssignmentStatement, IfStatement, ForStatement]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(indices={!r}, statements={!r})".format(
            type(self).__name__, self.indices, self.statements
        )


class WhileStatement(Node):
    def __init__(self, **kwargs):
        self.condition = None  # type: Optional[Union[Expression, Primary, ComponentRef]]
        self.statements = []  # type: List[Union[AssignmentStatement, IfStatement, ForStatement]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(condition={!r}, statements={!r})".format(
            type(self).__name__, self.condition, self.statements
        )


class Function(Node):
    def __init__(self, **kwargs):
        self.name = ""  # type: str
        self.arguments = []  # type: List[Union[Expression, Primary, ComponentRef, Array]]
        self.comment = ""  # type: str
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(name={!r}, arguments={!r})".format(
            type(self).__name__, self.name, self.arguments
        )


class Symbol(Node):
    """
    A mathematical variable or state of the model
    """

    ATTRIBUTES = [
        "value",
        "min",
        "max",
        "start",
        "fixed",
        "nominal",
        "unit",
        "quantity",
        "displayUnit",
    ]

    def __init__(self, **kwargs):
        # pylint: disable=invalid-name
        self.name = ""  # type: str
        self.type = ComponentRef()  # type: Union[ComponentRef, InstanceClass]
        self.prefixes = []  # type: List[str]
        self.replaceable = False  # type: bool
        self.final = False  # type: bool
        self.inner = False  # type: bool
        self.outer = False  # type: bool
        self.dimensions = [
            [Primary(value=None)]
        ]  # type: List[List[Union[Expression, Primary, ComponentRef]]]
        self.comment = ""  # type: str
        self.start = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef, Array]
        self.min = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef, Array]
        self.max = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef, Array]
        self.nominal = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef, Array]
        self.value = Primary(value=None)  # type: Union[Expression, Primary, ComponentRef, Array]
        self.fixed = Primary(value=False)  # type: Primary
        self.unit = Primary(value=None)  # type: Primary
        self.quantity = Primary(value=None)  # type: Primary
        self.displayUnit = Primary(value=None)  # type: Primary
        self.id = 0  # type: int
        self.order = 0  # type: int
        self.visibility = Visibility.PUBLIC  # type: Visibility
        self.class_modification = None  # type: Optional[ClassModification]
        self.parent = None  # type: Optional[Class]
        super().__init__(**kwargs)

    def full_reference(self) -> ComponentRef:
        return element_full_reference(self)

    @property
    def full_name(self) -> str:
        """Return fully-qualified name of this symbol"""
        return element_full_name(self)

    def __str__(self):
        return '{} {}, Type "{}"'.format(type(self).__name__, self.name, self.type)

    def __repr__(self):
        return "{}(name={!r}, type={!r})".format(type(self).__name__, self.name, self.type)


class EnumerationLiteral(Symbol):
    """An enumeration literal declared inside ``type E = enumeration(...)``.

    Subclasses :class:`Symbol` so that ``E.two`` resolves through the ordinary
    name-lookup path (which requires a ``constant`` Symbol).  Carries a 1-based
    *ordinal* value matching the declaration order (MLS 4.8.5).
    """

    def __init__(self, **kwargs):
        self.ordinal = None  # type: Optional[int]  # 1-based position in the enum
        super().__init__(**kwargs)
        self.prefixes = ["constant"]

    def __repr__(self):
        return "{}(name={!r}, ordinal={!r})".format(type(self).__name__, self.name, self.ordinal)


def is_enumeration(obj) -> bool:
    """Return True if *obj* is a Modelica enumeration type class (or instance)."""
    return getattr(obj, "enumeration", False)


class ComponentClause(Node):
    def __init__(self, **kwargs):
        self.prefixes = []  # type: List[str]
        self.type = ComponentRef()  # type: ComponentRef
        self.replaceable = False  # type: bool
        self.dimensions = [
            [Primary(value=None)]
        ]  # type: List[List[Union[Expression, Primary, ComponentRef]]]
        self.comment = []  # type: List[str]
        self.symbol_list = []  # type: List[Symbol]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(prefixes={!r}, type={}, dimensions={!r}, symbol_list={!r})".format(
            type(self).__name__, self.prefixes, self.type, self.dimensions, self.symbol_list
        )

    def __str__(self):
        pre = " ".join([str(s) for s in self.prefixes])
        return "{} {!r} {!r} {!r}".format(pre, self.type, self.dimensions, self.symbol_list)


class EquationSection(Node):
    def __init__(self, **kwargs):
        self.initial = False  # type: bool
        self.equations = []  # type: List[Union[Equation, IfEquation, ForEquation, ConnectClause]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(initial={!r}, equations={!r})".format(
            type(self).__name__, self.initial, self.equations
        )


class AlgorithmSection(Node):
    def __init__(self, **kwargs):
        self.initial = False  # type: bool
        self.statements = []  # type: List[Union[AssignmentStatement, IfStatement, ForStatement]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(initial={!r}, statements={!r})".format(
            type(self).__name__, self.initial, self.statements
        )


class ImportClause(Node):
    def __init__(self, **kwargs):
        self.components = []  # type: List[ComponentRef]
        self.short_name = ""  # type: str
        self.unqualified = False  # type: bool
        # Comments are rare, so ignore
        super().__init__(**kwargs)

    def __str__(self):
        star = ""
        if self.unqualified:
            star = ".*"
        return "import {!r}{!r}{!r}".format(self.short_name, self.components, star)

    def __repr__(self):
        return "{}(components={}, short_name={!r}), unqualfied={!r}".format(
            type(self).__name__, self.components, self.short_name, self.unqualified
        )


class ElementModification(Node):
    def __init__(self, **kwargs):
        self.component = ComponentRef()  # type: ComponentRef
        self.modifications = (
            []
        )  # type: List[Union[Primary, Expression, ClassModification, Array, ComponentRef]]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(component={}, modifications={!r})".format(
            type(self).__name__, self.component, self.modifications
        )

    def __str__(self):
        return "{!r} = {!r}".format(self.component, self.modifications)


class ShortClassDefinition(Node):
    def __init__(self, **kwargs):
        self.name = ""  # type: str
        self.type = ""  # type: str
        self.component = ComponentRef()  # type: ComponentRef
        self.class_modification = ClassModification()  # type: ClassModification
        self.replaceable = False  # type: bool
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(name={!r}, type={!r}, component={}, class_modification={!r})".format(
            type(self).__name__, self.name, self.type, self.component, self.class_modification
        )

    def __str__(self):
        return "{!r} = {!r}".format(self.name, self.component)


class ElementReplaceable(Node):
    def __init__(self, **kwargs):
        # TODO, add fields ?
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}()".format(type(self).__name__)


class ClassModification(Node):
    def __init__(self, **kwargs):
        self.arguments = []  # type: List[ClassModificationArgument]
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(arguments={!r})".format(type(self).__name__, self.arguments)


class ClassModificationArgument(Node):
    def __init__(self, **kwargs):
        self.value = (
            []
        )  # type: List[Union[ElementModification, ComponentClause, ShortClassDefinition]]
        self.scope = None  # type: Optional[Union[Class, InstanceClass]]
        self.redeclare = False
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(value={!r}, scope={!r}, redeclare={!r})".format(
            type(self).__name__, self.value, self.scope, self.redeclare
        )

    def __deepcopy__(self, memo):
        _scope, _deepcp = self.scope, self.__deepcopy__
        self.scope, self.__deepcopy__ = None, None
        new = copy.deepcopy(self, memo)
        self.scope, self.__deepcopy__ = _scope, _deepcp
        new.scope, new.__deepcopy__ = _scope, _deepcp
        return new


class ExtendsClause(Node):
    def __init__(self, **kwargs):
        self.component = None  # type: Optional[ComponentRef]
        self.class_modification = None  # type: Optional[ClassModification]
        self.visibility = Visibility.PUBLIC  # type: Visibility
        self.scope = None  # type: Optional[Union[Class, InstanceClass]]
        # True when created from `class extends X` syntax (MLS §7.3.1).
        # The base class X must be resolved in the inherited scope of the enclosing
        # class, not the local scope (which already contains the redeclaration).
        self.is_class_extends = False  # type: bool
        super().__init__(**kwargs)

    def __repr__(self):
        return "{}(component={}, class_modification={!r}, visibility={!r})".format(
            type(self).__name__, self.component, self.class_modification, self.visibility
        )


class Class(Node):
    BUILTIN = ("Real", "Integer", "String", "Boolean")

    def __init__(self, **kwargs):
        self.name = None  # type: Optional[str]
        self.imports = OrderedDict()  # type: OrderedDict[str, Union[ImportClause, ComponentRef]]
        self.extends = []  # type: List[Union[ExtendsClause, InstanceClass]]
        self.encapsulated = False  # type: bool
        self.partial = False  # type: bool
        self.final = False  # type: bool
        self.replaceable = False  # type: bool
        self.type = ""  # type: str
        self.comment = ""  # type: str
        self.classes = OrderedDict()  # type: dict[str, Class]
        self.symbols = OrderedDict()  # type: OrderedDict[str, Symbol]
        self.functions = OrderedDict()  # type: OrderedDict[str, Class]
        self.initial_equations = []  # type: List[Union[Equation, ForEquation]]
        self.equations = []  # type: List[Union[Equation, ForEquation, ConnectClause]]
        self.initial_statements = (
            []
        )  # type: List[Union[AssignmentStatement, IfStatement, ForStatement]]
        self.statements = []  # type: List[Union[AssignmentStatement, IfStatement, ForStatement]]
        self.annotation = None  # type: Optional[ClassModification]
        self.parent = None  # type: Optional[Class]
        self.visibility = Visibility.PUBLIC  # type: Visibility
        self.is_short_class_definition = False  # type: bool
        self.enumeration = False  # type: bool  # True iff this is an enumeration type

        super().__init__(**kwargs)

    def full_reference(self) -> ComponentRef:
        return element_full_reference(self)

    @property
    def full_name(self) -> str:
        """Return fully-qualified lexical name of this class"""
        return element_full_name(self)

    def _extend(self, other: "Class") -> None:
        for class_name in other.classes.keys():
            if class_name in self.classes.keys():
                self.classes[class_name]._extend(other.classes[class_name])
            else:
                self.classes[class_name] = other.classes[class_name]

    def _update_parent_refs(self) -> None:
        for c in self.classes.values():
            c.parent = self
            c._update_parent_refs()

    def update_classes(self, other: dict[str, "Class"]) -> None:
        for class_ in other.values():
            self.add_class(class_)

    @property
    def root(self):
        if self.parent is None:
            return self
        else:
            return self.parent.root

    def ancestors(self) -> List["Class"]:
        """Return a list of all ancestor classes, farthest first."""
        ancestors = []
        current = self.parent
        while current is not None:
            ancestors.append(current)
            current = current.parent
        ancestors.reverse()
        return ancestors

    def copy_including_children(self):
        return copy.deepcopy(self)

    def add_class(self, c: "Class") -> None:
        """
        Add a (sub)class to this class.

        :param c: (Sub)class to add.
        """
        self.classes[c.name] = c
        c.parent = self

    def remove_class(self, c: "Class") -> None:
        """
        Removes a (sub)class from this class.

        :param c: (Sub)class to remove.
        """
        del self.classes[c.name]
        c.parent = None

    def add_symbol(self, s: Symbol) -> None:
        """
        Add a symbol to this class.

        :param s: Symbol to add.
        """
        self.symbols[s.name] = s

    def remove_symbol(self, s: Symbol) -> None:
        """
        Removes a symbol from this class.

        :param s: Symbol to remove.
        """
        del self.symbols[s.name]

    def add_equation(self, e: Equation) -> None:
        """
        Add an equation to this class.

        :param e: Equation to add.
        """
        self.equations.append(e)

    def remove_equation(self, e: Equation) -> None:
        """
        Removes an equation from this class.

        :param e: Equation to remove.
        """
        self.equations.remove(e)

    def add_initial_equation(self, e: Equation) -> None:
        """
        Add an initial equation to this class.

        :param e: Equation to add.
        """
        self.initial_equations.append(e)

    def remove_initial_equation(self, e: Equation) -> None:
        """
        Removes an initial equation from this class.

        :param e: Equation to remove.
        """
        self.initial_equations.remove(e)

    def __deepcopy__(self, memo):
        # Avoid copying the entire tree
        if self.parent is not None and self.parent not in memo:
            memo[id(self.parent)] = self.parent

        _deepcp = self.__deepcopy__
        self.__deepcopy__ = None
        new = copy.deepcopy(self, memo)
        self.__deepcopy__ = _deepcp
        new.__deepcopy__ = _deepcp
        return new

    def __repr__(self):
        return "{}(name={!r}, type={!r})".format(type(self).__name__, self.name, self.type)

    def __str__(self):
        return '{} {}, Type "{}"'.format(type(self).__name__, self.name, self.type)


def element_name_tuple(element: Union[Class, Symbol]) -> tuple[str]:
    """Return fully-qualified lexical name of an element as a tuple of names"""
    names = []
    current = element
    while current.parent is not None:
        names.append(current.name)
        current = current.parent
    return tuple(reversed(names))


def element_full_name(element: Union[Class, Symbol]) -> str:
    """Return fully-qualified lexical name of an element"""
    return ".".join(element_name_tuple(element))


def element_full_reference(element: Union[Class, Symbol]) -> ComponentRef:
    """Return fully-qualified component reference to element"""
    name_tuple = element_name_tuple(element)
    return ComponentRef.from_tuple(name_tuple) if name_tuple else ComponentRef()


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
        ast_ref: Optional[Union[Class, Symbol]] = None,
        modification_environment: Optional[ClassModification] = None,
        parent_instance: Optional["InstanceClass"] = None,
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
            self.modification_environment = ClassModification()

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
            self.type = ComponentRef()  # The default in Symbol

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
            if isinstance(value, (list, dict, ClassModification)):
                new_self.__dict__[key] = copy.copy(value)
            if isinstance(value, ClassModification):
                new_self.__dict__[key].arguments = copy.copy(value.arguments)
        return new_self

    def __repr__(self):
        return f"name={self.name!r}, ast_ref={self.ast_ref!r}, modification_environment={self.modification_environment!r}"


def element_instance_name_tuple(element: InstanceElement) -> tuple[str]:
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


class InstanceClass(InstanceElement, Class):
    """
    Class used during instantiation and flattening of the model.
    """

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def __repr__(self):
        return f"{type(self).__name__}({super().__repr__()!s})"


class InstanceSymbol(InstanceElement, Symbol):
    """
    Symbol used during instantiation and flattening of the model.
    """

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def __repr__(self):
        return f"{type(self).__name__}({super().__repr__()!s})"


class Tree(Class):
    """
    The root class of the class tree
    """

    BUILTIN_TYPES = {
        "Real": Symbol(
            name="Real",
            type=ComponentRef(name="Real"),
            start=Primary(value=0.0),
            min=Primary(value=-math.inf),
            max=Primary(value=math.inf),
            nominal=Primary(value=None),
            fixed=Primary(value=False),  # True for parameters and constants
            unit=Primary(value=""),
            quantity=Primary(value=""),
            displayUnit=Primary(value=""),
            # TODO: unbounded from spec is missing in Symbol
            # TODO: stateSelect from spec is missing in Symbol
            class_modification=ClassModification(),
        ),
        "Integer": Symbol(
            name="Integer",
            type=ComponentRef(name="Integer"),
            start=Primary(value=0.0),
            min=Primary(value=-sys.maxsize),
            max=Primary(value=sys.maxsize),
            fixed=Primary(value=False),  # True for parameters and constants
            quantity=Primary(value=""),
            class_modification=ClassModification(),
        ),
        "Boolean": Symbol(
            name="Boolean",
            type=ComponentRef(name="Boolean"),
            start=Primary(value=0.0),
            fixed=Primary(value=False),  # True for parameters and constants
            quantity=Primary(value=""),
            class_modification=ClassModification(),
        ),
        "String": Symbol(
            name="String",
            type=ComponentRef(name="String"),
            start=Primary(value=0.0),
            fixed=Primary(value=False),  # True for parameters and constants
            quantity=Primary(value=""),
            class_modification=ClassModification(),
        ),
    }

    # Predefined enumeration types per MLS 4.8.4; maps name → tuple of literal names
    BUILTIN_ENUM_TYPES = {
        "StateSelect": ("never", "avoid", "default", "prefer", "always"),
    }

    # Predefined opaque types (no attributes/literals); MLS 16.2 Clock
    BUILTIN_OPAQUE_TYPES = frozenset({"Clock"})

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.type = "package"
        self._create_builtins()

    def _create_builtins(self):
        """Add builtins to root of tree"""
        for name, symbol in self.BUILTIN_TYPES.items():
            new_symbol = copy.deepcopy(symbol)
            type_class = Class(name=name, type="type", parent=self)
            new_symbol.parent = type_class
            type_class.symbols[name] = new_symbol
            self.classes[name] = type_class
        for name, literals in self.BUILTIN_ENUM_TYPES.items():
            type_class = Class(name=name, type="type", parent=self)
            type_class.enumeration = True
            for ordinal, literal in enumerate(literals, start=1):
                enum_sym = EnumerationLiteral(
                    name=literal,
                    type=ComponentRef(name=name),
                    ordinal=ordinal,
                    class_modification=ClassModification(),
                )
                enum_sym.parent = type_class
                type_class.symbols[literal] = enum_sym
            self.classes[name] = type_class
        for name in self.BUILTIN_OPAQUE_TYPES:
            type_class = Class(name=name, type="type", parent=self)
            self.classes[name] = type_class

    def extend(self, other: "Tree") -> None:
        self._extend(other)
        self.update_parent_refs()

    def update_parent_refs(self) -> None:
        self._update_parent_refs()

    def __repr__(self):
        return "{}(classes={!r})".format(type(self).__name__, self.classes)
