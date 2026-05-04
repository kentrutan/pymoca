#!/usr/bin/env python
"""
Modelica flattening — MLS 5.6.2

Entry: flatten_instance(instance) → _flatten_instance()
"""

from __future__ import absolute_import, division, print_function, unicode_literals

import copy  # TODO
from collections import defaultdict
from typing import Optional, Set, Union, cast

from ._base import ModelicaSemanticError, NameLookupError, TreeListener, TreeWalker
from ._instantiation import InstanceTree, _instantiate_class
from ._name_lookup import _find_name
from .. import ast


def flatten_instance(
    instance: ast.InstanceClass,
    keep_connectors: bool = False,
) -> ast.InstanceClass:
    """Flatten an instance class

    :param instance: The instance class to flatten
    :param keep_connectors: Whether to keep connectors in the top-level flattened class
    :return: The flattened class
    """
    if not isinstance(instance, ast.InstanceClass):
        raise TypeError(f"Expected InstanceClass, got {type(instance)}")

    flat_class = ast.InstanceClass(
        name=_flatten_name(instance),
        comment=instance.comment,
        type=instance.type,
        ast_ref=instance.ast_ref,
        parent=instance.parent,
        parent_instance=instance.parent_instance,
        fully_instantiated=instance.fully_instantiated,
        partially_instantiated=instance.partially_instantiated,
    )

    # Populate classes and extends for name lookup
    for name, class_ in instance.classes.items():
        class_copy = copy.copy(class_)
        class_copy.parent_instance = flat_class
        flat_class.classes[name] = class_copy
    for extends in instance.extends:
        extends_copy = copy.copy(extends)
        extends_copy.parent_instance = flat_class
        flat_class.extends.append(extends_copy)

    _flatten_instance(instance, flat_class)
    return flat_class


def _flatten_instance(
    instance: ast.InstanceClass,
    flat_class: ast.InstanceClass,
    prefix: str = "",
    keep_connectors: bool = False,
) -> None:
    """Flatten an instance class

    :param instance: The instance class to flatten
    :param flat_class: The flattened class
    :param prefix: The prefix for the current instance
    :param keep_connectors: Whether to keep connectors in the flattened class
    :return: The flattened instance class

    The passed instance may be updated by fully instantiating elements as needed.
    We allow the top-level flattened instance to retain connectors so that we can
    flatten a set of components and connect them together later.
    """
    # Outline of spec 3.5 section 5.6.2 *Generation of the flat equation system*:
    # Definitions/Notes:
    #   - Simple Type: Integer, Real, Boolean, String, enumeration, Clock, or External
    #     Object
    #   - Mark steps TODO that we will postpone on the first pass
    #
    # 1 Recursively walk the tree, beginning at the class to be flattened, and process
    # components with the following items used throughout:
    # * A. "all references by name in conditional declarations, modifications, dimension
    #   definitions, annotations, equations and algorithms are resolved to the real
    #   instance to which they are referring to ... [Either the referenced instance
    #   belongs to the model to be simulated the path starts at the model itself, or if
    #   not, it starts at the unnamed root of the instance tree, e.g. in case of a
    #   constant in a package.]"
    # * B. Names are replaced by the global unique identifier of the instance "[This
    #   identifier is normally constructed from the names of the instances along a path
    #   in the instance tree (and omitting the unnamed nodes of extends clauses),
    #   separated by dots. Either the referenced instance belongs to the model to be
    #   simulated the path starts at the model itself, or if not, it starts at the
    #   unnamed root of the instance tree, e.g. in case of a constant in a package.]"
    # * C. Classes and symbols are instantiated as needed (if not a fully instantiated instance)
    # * D. Classes are ignored unless needed by components
    # * E. Function calls encountered in the above are processed as needed: (TODO)
    #   - Fill the call with default arguments (section 12.4.1)
    #   - Look up the function in the instance tree
    #   - Recursively flatten the function call
    #   - Add to the list of functions
    #
    # 1.1 Insert components into the variables list
    # 1.2 Evaluate the conditional declaration expression and mark the declaration for
    #     deletion (TODO)
    # 1.3 Resolve dimensions, including enclosing instances (TODO)
    # 1.4 Resolve modifications of value attributes of simple types and records (TODO: records)
    # 1.5 Resolve modifications of other attributes of simple types
    # 1.6 Recursively "handle" non-simple types
    # 1.7 Resolve references in equations and algorithms
    # 1.8 Recursively "handle" unnamed extends instances
    # 1.9 Check that all references now point to a valid instance (not conditionally false)
    #
    # 2 Generate connect equations for all connections in the flattened tree
    #
    # 3 Process transitions in the flattened tree (TODO)

    # 1 Recursively walk the tree, beginning at the class to be flattened
    for name, symbol in list(instance.symbols.items()):

        assert isinstance(name, str)
        # Steps A-D
        if name in InstanceTree.BUILTIN_TYPES and prefix:
            flat_name = prefix
        elif prefix:
            flat_name = prefix + "." + name
        else:
            flat_name = name
        flat_symbol = copy.copy(symbol)
        if symbol.type.name in InstanceTree.BUILTIN_TYPES:
            flat_symbol.name = flat_name
            flat_class.symbols[flat_name] = flat_symbol
        elif not isinstance(flat_symbol.type, ast.InstanceClass):
            resolved = _resolve_name(flat_symbol.type, instance, flat_class)
            is_class = isinstance(resolved, ast.InstanceClass)
            flat_symbol.type = resolved if is_class else resolved.parent

        # 1.2 Evaluate the conditional declaration expression and mark the declaration for
        #     deletion (TODO)
        _evaluate_conditional_declarations(flat_symbol, flat_class)

        # 1.3 Resolve dimensions, including enclosing instances
        _resolve_dimensions(flat_symbol, flat_class)

        # 1.4 Resolve modifications of value attributes of simple types and records
        # 1.5 Resolve modifications of other attributes of simple types
        if flat_symbol.type.name in InstanceTree.BUILTIN_TYPES or "record" in flat_symbol.prefixes:
            _resolve_modifications(flat_symbol, flat_class)
        else:
            # 1.6 Recursively "handle" non-simple types
            _flatten_instance(flat_symbol.type, flat_class, flat_name)

    # 1.7 Resolve references in equations and algorithms
    _resolve_equation_and_algorithm_references(flat_class)

    # 1.8 Recursively "handle" unnamed extends instances
    for extends in instance.extends:
        _flatten_instance(extends, flat_class, prefix)

    # 1.9 Check that all references now point to a valid instance (not conditionally false)
    _check_all_references_valid(flat_class)

    # 2. Generate connect equations for all connections in the flattened tree
    if not keep_connectors:
        _generate_connect_equations(flat_class)

    # 3. Process transitions in the flattened tree
    _process_transitions(flat_class)


def _evaluate_conditional_declarations(symbol: ast.InstanceSymbol, parent: ast.Class):
    # TODO: 1.2 Evaluate the conditional declaration expression
    pass


def _resolve_dimensions(symbol: ast.InstanceSymbol, parent: ast.Class):
    # TODO: 1.3 Resolve dimensions, including enclosing instances
    pass


def _resolve_modifications(symbol: ast.InstanceSymbol, flat_class: ast.InstanceClass) -> None:
    """Resolve modifications of a symbol
    :param symbol: The symbol to resolve modifications for
    :param flat_class: The flattened class
    :return: None
    """
    # TODO: Resolve modifications of records
    if "record" in symbol.prefixes:
        raise NotImplementedError("Record modifications not implemented yet")

    # TODO: Instantiation should move type class modifications down to the builtin
    if isinstance(symbol.type, ast.ComponentRef):
        # type class, modifications at this level
        modification_environment = symbol.modification_environment
    else:
        modification_environment = symbol.type.symbols[symbol.type.name].modification_environment

    for arg in modification_environment.arguments:
        assert isinstance(arg.value, ast.ElementModification)
        if arg.value.component == "value" and "record" in symbol.prefixes:
            # TODO: record value modification
            continue
        _resolve_modification_attribute(symbol, arg)


def _resolve_modification_attribute(
    symbol: ast.InstanceSymbol,
    arg: ast.ClassModificationArgument,
):
    # The spec says the modifications are resolved by creating equations, but we are
    # going to set the symbol attributes now and create equations for them in another
    # pass.
    # Process the various ElementModification types
    # type: List[Union[Primary, Expression, ClassModification, Array, ComponentRef]]
    mod = cast(ast.ElementModification, arg.value)
    value = mod.modifications[0]
    assert not isinstance(value, ast.ClassModification)
    if isinstance(value, ast.Primary):
        value = value.value
    elif isinstance(value, ast.ComponentRef):
        assert isinstance(symbol.parent, ast.InstanceClass)
        value = _resolve_name(value, arg.scope, symbol.parent)
        value = cast(ast.InstanceSymbol, value)
    elif isinstance(value, ast.Array):
        # TODO: Handle array modifications
        raise NotImplementedError("Array modifications not implemented yet")
        # value = _resolve_array(value, arg.scope, symbol.parent)
    elif isinstance(value, ast.Expression):
        value = _resolve_expression(
            value,
            arg.scope,
            symbol.parent_instance,
            current_instances=None,
            current_extends=None,
            instantiate_in_place=False,
        )

    setattr(symbol, mod.component.name, value)


class ExpressionEvaluator(TreeListener):
    """Calculate an ast.Expression tree containing only Primary and ComponentRef operands

    TODO: Ensure expression evaluation is according to Modelica spec"""

    # How to transform Modelica operators to Python
    transform_operator = defaultdict(
        None,
        {
            "+": "+",
            "-": "-",
            "*": "*",
            "/": "/",
            "^": "**",
            "and": "and",
            "or": "or",
            "not": "not",
            "<": "<",
            "<=": "<=",
            ">": ">",
            ">=": ">=",
            "==": "==",
            "<>": "!=",
        },
    )

    def __init__(
        self,
        scope: ast.InstanceClass,
        flat_class: ast.InstanceClass,
        current_instances: Optional[Set[ast.InstanceClass]],
        current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
        instantiate_in_place: bool,
    ):
        self.scope = scope
        self.flat_class = flat_class
        self.current_instances = current_instances
        self.current_extends = current_extends
        self.instantiate_in_place = instantiate_in_place

        self.result = None
        super().__init__()

    def enterExpression(self, tree: ast.Expression):
        operator = self.transform_operator[str(tree.operator)]
        if operator is None:
            raise NotImplementedError(f"Unsupported operator {tree.operator} in {tree:r}")
        operands = []
        for operand in tree.operands:
            if isinstance(operand, ast.ComponentRef):
                operand = _resolve_name(
                    operand,
                    self.scope,
                    self.flat_class,
                    self.current_instances,
                    self.current_extends,
                    self.instantiate_in_place,
                )
            if not isinstance(operand, (ast.Primary, ast.Symbol)):
                raise NotImplementedError(f"Expression operand type not implemented: {operand!r}")
            operands.append(operand)

        if len(operands) == 1:
            expr_parts = (operator, operands[0].value)
        else:
            expr_parts = (operands[0].value, operator, operands[1].value)
        expr = " ".join(str(part) for part in expr_parts)
        try:
            self.result = eval(expr)
        except Exception:
            raise ModelicaSemanticError(f"Unable to evaluate expression: {expr}")


def _resolve_expression(
    expr: ast.Expression,
    scope: ast.InstanceClass,
    flat_class: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> Optional[Union[int, float, bool, str]]:
    """Calculate the given expression or return None if not possible"""
    assert isinstance(expr, ast.Expression)
    listener = ExpressionEvaluator(
        scope=scope,
        flat_class=flat_class,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )
    walker = TreeWalker()
    walker.walk(listener, expr)
    return listener.result


def _resolve_equation_and_algorithm_references(flat_class: ast.InstanceClass):
    # TODO: 1.7 Resolve references in equations and algorithms
    pass


def _check_all_references_valid(flat_class: ast.InstanceClass):
    # TODO: 1.9 Check that all references now point to a valid instance (not conditionally false)
    pass


def _generate_connect_equations(flat_class: ast.InstanceClass):
    # TODO: 2. Generate connect equations for all connections in the flattened tree
    pass


def _process_transitions(flat_class: ast.InstanceClass):
    # TODO: 3. Process transitions in the flattened tree
    pass


def _flatten_name(
    element: Union[ast.Class, ast.InstanceClass, ast.Symbol, ast.InstanceSymbol],
    remove_prefix: str = "",
) -> str:
    """Flatten the instance full_reference() into a name str

    :param instance: The instance having the name to flatten
    :param remove_prefix: The prefix to remove from the name
    :return: The flattened name
    """
    if isinstance(element, ast.InstanceElement):
        assert element.ast_ref is not None
        global_ref = element.ast_ref.full_reference()
    else:
        global_ref = element.full_reference()
    for name in remove_prefix.split("."):
        if global_ref.name != name:
            break
        global_ref = global_ref.child[0]
    return str(global_ref)


def _resolve_name(
    name: Union[str, ast.ComponentRef],
    scope: ast.InstanceClass,
    flat_class: ast.InstanceClass,
) -> Union[ast.InstanceClass, ast.InstanceSymbol]:
    """Flatten and resolve a name reference"""
    # * A. "all references by name in conditional declarations, modifications, dimension
    #   definitions, annotations, equations and algorithms are resolved to the real
    #   instance to which they are referring to ... [Either the referenced instance
    #   belongs to the model to be simulated the path starts at the model itself, or if
    #   not, it starts at the unnamed root of the instance tree, e.g. in case of a
    #   constant in a package.]"
    # * B. Names are replaced by the global unique identifier of the instance "[This
    #   identifier is normally constructed from the names of the instances along a path
    #   in the instance tree (and omitting the unnamed nodes of extends clauses),
    #   separated by dots. Either the referenced instance belongs to the model to be
    #   simulated the path starts at the model itself, or if not, it starts at the
    #   unnamed root of the instance tree, e.g. in case of a constant in a package.]"
    # * C. Classes and symbols are instantiated as needed (if not a fully instantiated instance)
    # * D. Classes are ignored unless needed by components
    # * E. Function calls encountered in the above are processed as needed: (TODO)
    #   - Fill the call with default arguments (section 12.4.1)
    #   - Look up the function in the instance tree
    #   - Recursively flatten the function call
    #   - Add to the list of functions

    if not scope.fully_instantiated:
        # Fully instantiate the scope class to get instantiated symbol
        scope = _instantiate_class(scope, ast.ClassModification(), scope.parent)

    found = _find_name(name, scope)
    if found is None:
        raise NameLookupError(f"Unable to resolve {name} in scope {scope.full_name}")
    is_symbol = isinstance(found, ast.Symbol)

    flat_name = _flatten_name(found, flat_class.name)

    # Check if the name is already resolved
    if is_symbol and flat_name in flat_class.symbols:
        # Return the already-resolved symbol
        return flat_class.symbols[flat_name]
    elif flat_name in flat_class.classes:
        # Return the already-resolved class
        return flat_class.classes[flat_name]

    if not isinstance(found, ast.InstanceElement) or not found.fully_instantiated:
        if is_symbol:
            # TODO: Implement direct instantiation of symbols
            # Fully instantiate the symbol parent class to get instantiated symbol
            assert found.parent is not None
            found.parent = _instantiate_class_update_name(found.parent, scope, flat_name)
            found = found.parent.symbols[found.name]
            flat_class.symbols[flat_name] = found
        else:  # class
            found = _instantiate_class_update_name(found, scope, flat_name)
            flat_class.classes[flat_name] = found

    return found


def _instantiate_class_update_name(
    class_: Union[ast.Class, ast.InstanceClass],
    parent: ast.InstanceClass,
    flat_name: str,
) -> ast.InstanceClass:
    assert class_.parent is not None
    instance = _instantiate_class(class_, ast.ClassModification(), parent)
    # Update the instance tree with the fully instantiated class
    assert instance.name is not None
    assert instance.parent is not None
    # If "unnamed" extends class, it reverts back to original name
    instance.name = flat_name if instance.name else instance.ast_ref.name
    return instance
