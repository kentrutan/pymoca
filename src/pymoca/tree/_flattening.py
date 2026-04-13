#!/usr/bin/env python
"""
Modelica flattening — MLS 5.6.2

Entry: flatten_instance(instance) → _flatten_instance()
Adapter: flatten_to_tree(root, class_name) → ast.Tree (for backend compatibility)
"""

from __future__ import absolute_import, division, print_function, unicode_literals

import copy
import operator
import sys
from collections import OrderedDict
from typing import Optional, Set, Union, cast

from ._base import ModelicaSemanticError, NameLookupError, TreeListener, TreeWalker
from ._instantiation import (
    InstanceTree,
    _get_lexical_parent_instance,
    _instantiate_class,
    instantiate,
)
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

    flat_class = _create_partial_flat_instance(instance)
    functions = OrderedDict()
    _flatten_instance(instance, flat_class, functions=functions)
    _flatten_discovered_functions(functions, flat_class)
    _check_all_references_valid(flat_class)
    _process_transitions(flat_class)
    if not keep_connectors:
        _generate_connect_equations(flat_class)
    return flat_class


def _create_partial_flat_instance(instance: ast.InstanceClass) -> ast.InstanceClass:
    flat_class = ast.InstanceClass(
        name=_flatten_name(instance),
        comment=instance.comment,
        type=instance.type,
        ast_ref=instance.ast_ref,
        parent=instance.parent,
        parent_instance=instance.parent_instance,
        instantiation_state=instance.instantiation_state,
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

    return flat_class


def _flatten_instance(
    instance: ast.InstanceClass,
    flat_class: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]] = None,
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]] = None,
    instantiate_in_place: bool = True,
    prefix: str = "",
    functions: Optional[OrderedDict] = None,
) -> None:
    """Flatten an instance class

    :param instance: The instance class to flatten
    :param flat_class: The flattened class
    :param current_instances: ...
    :param current_extends: ...
    :param instantiate_in_place: ...
    :param prefix: The prefix for the current instance
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
    # * E. Function calls encountered in the above are processed as needed:
    #   - Fill the call with default arguments (section 12.4.1) (TODO)
    #   - Look up the function in the instance tree
    #   - Recursively flatten the function call
    #   - Add to the list of functions
    #
    # 1.1 Insert components into the variables list
    # 1.2 Evaluate the conditional declaration expression and mark the declaration for
    #     deletion (TODO)
    # 1.3 Resolve dimensions, including enclosing instances
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

    # 1.1–1.6 Process local symbols per MLS 5.6.2
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
        # Strip input/output prefixes from nested symbols — these are
        # connector-local causality markers (MLS 9.1.1) and should not
        # propagate to the parent model's flattened namespace.
        if prefix:
            flat_symbol.prefixes = [p for p in flat_symbol.prefixes if p not in ("input", "output")]
        if symbol.type.name in InstanceTree.BUILTIN_TYPES:
            flat_symbol.name = flat_name
            flat_class.symbols[flat_name] = flat_symbol
        elif not isinstance(flat_symbol.type, ast.InstanceClass):
            resolved = _resolve_name(
                flat_symbol.type,
                instance,
                flat_class,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )
            is_class = isinstance(resolved, ast.InstanceClass)
            flat_symbol.type = resolved if is_class else resolved.parent

        # 1.2 Evaluate the conditional declaration expression and mark the declaration for
        #     deletion (TODO)
        _evaluate_conditional_declarations(flat_symbol, flat_class)

        # 1.3 Resolve dimensions, including enclosing instances
        _resolve_dimensions(
            flat_symbol,
            instance,
            flat_class,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )

        # 1.4 Resolve modifications of value attributes of simple types and records
        # 1.5 Resolve modifications of other attributes of simple types
        if flat_symbol.type.name in InstanceTree.BUILTIN_TYPES or "record" in flat_symbol.prefixes:
            _resolve_modifications(
                flat_symbol,
                flat_class,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                functions=functions,
            )
        else:
            # 1.6 Recursively "handle" non-simple types
            symbols_before_recursive = set(flat_class.symbols.keys())
            _flatten_instance(
                flat_symbol.type,
                flat_class,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                prefix=flat_name,
                functions=functions,
            )

            # Propagate outer symbol dimensions to inner symbols (MLS 5.6.2 step 1.3)
            # Always propagate (including scalar [[None]]) to preserve nesting depth
            # for backend indexing.  Only propagate to symbols added by this recursive
            # call to avoid double-prepending when extends chains share component names.
            outer_dims = flat_symbol.dimensions
            flat_name_prefix = flat_name + "."
            for sym_name in set(flat_class.symbols.keys()) - symbols_before_recursive:
                if sym_name.startswith(flat_name_prefix):
                    sym = flat_class.symbols[sym_name]
                    sym.dimensions = outer_dims + sym.dimensions

    # 1.7 Resolve references in equations and algorithms
    _collect_and_resolve_equations(instance, flat_class, prefix, functions=functions)

    # 1.8 Recursively "handle" unnamed extends instances
    for extends in instance.extends:
        _flatten_instance(
            extends,
            flat_class,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            prefix=prefix,
            functions=functions,
        )

    # Steps 1.9, 2, and 3 are done outside the recursion in the caller


def _evaluate_conditional_declarations(symbol: ast.InstanceSymbol, parent: ast.Class):
    # TODO: 1.2 Evaluate the conditional declaration expression
    pass


def _resolve_dimensions(
    symbol: ast.InstanceSymbol,
    instance: ast.InstanceClass,
    flat_class: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> None:
    """Resolve parametric array dimensions to concrete integers.

    Each element in symbol.dimensions is a list of dimension specifiers.
    ComponentRef and Expression elements are resolved to Primary(value=int(...)).
    """
    for dim_list in symbol.dimensions:
        for i, elem in enumerate(dim_list):
            if isinstance(elem, ast.Primary) or isinstance(elem, ast.Slice):
                continue
            elif isinstance(elem, ast.ComponentRef):
                resolved = _resolve_name(
                    elem,
                    instance,
                    flat_class,
                    current_instances=current_instances,
                    current_extends=current_extends,
                    instantiate_in_place=instantiate_in_place,
                )
                if isinstance(resolved, ast.InstanceSymbol) and isinstance(
                    resolved.value, (int, float)
                ):
                    dim_list[i] = ast.Primary(value=int(resolved.value))
            elif isinstance(elem, ast.Expression):
                result = _resolve_expression(
                    elem,
                    instance,
                    flat_class,
                    current_instances=current_instances,
                    current_extends=current_extends,
                    instantiate_in_place=instantiate_in_place,
                )
                if isinstance(result, (int, float)):
                    dim_list[i] = ast.Primary(value=int(result))


def _resolve_modifications(
    symbol: ast.InstanceSymbol,
    flat_class: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    functions: Optional[OrderedDict] = None,
) -> None:
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
        _resolve_modification_attribute(
            symbol,
            arg,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            functions=functions,
        )


def _resolve_modification_attribute(
    symbol: ast.InstanceSymbol,
    arg: ast.ClassModificationArgument,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    functions: Optional[OrderedDict] = None,
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
        value = _resolve_name(
            value,
            arg.scope,
            symbol.parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
        value = cast(ast.InstanceSymbol, value)
    elif isinstance(value, (ast.Array, ast.Expression)):
        # Discover function calls inside Array elements and Expressions so
        # that the function flattening pass can include them in the output.
        # Without this, functions referenced only in modifications are lost.
        if functions is not None:
            scope = (
                arg.scope if isinstance(arg.scope, ast.InstanceClass) else symbol.parent_instance
            )
            func_resolver = _FunctionCallResolver(scope, functions)
            walker = TreeWalker()
            walker.walk(func_resolver, value)
    if isinstance(value, ast.Expression):
        # Try to evaluate constant expressions (e.g. +1 → 1, -1.0 → -1.0);
        # keep the original Expression if evaluation fails (e.g. references
        # to non-constant variables or unsupported operators).
        try:
            result = _resolve_expression(
                value,
                arg.scope,
                symbol.parent_instance,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )
            if result is not None:
                value = result
        except (NotImplementedError, ModelicaSemanticError):
            pass  # keep original ast.Expression

    setattr(symbol, mod.component.name, value)


class ExpressionEvaluator(TreeListener):
    """Calculate an ast.Expression tree containing only Primary and ComponentRef operands

    TODO: Ensure expression evaluation is according to Modelica spec"""

    # Unary and binary operator callables for Modelica → Python dispatch
    unary_operator = {
        "+": operator.pos,
        "-": operator.neg,
        "not": operator.not_,
    }
    binary_operator = {
        "+": operator.add,
        "-": operator.sub,
        "*": operator.mul,
        "/": operator.truediv,
        "^": operator.pow,
        "and": operator.and_,
        "or": operator.or_,
        "<": operator.lt,
        "<=": operator.le,
        ">": operator.gt,
        ">=": operator.ge,
        "==": operator.eq,
        "<>": operator.ne,
    }

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
        op_str = str(tree.operator)
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

        try:
            if len(operands) == 1:
                op_func = self.unary_operator.get(op_str)
                if op_func is None:
                    raise NotImplementedError(f"Unsupported unary operator {op_str!r} in {tree!r}")
                self.result = op_func(operands[0].value)
            else:
                op_func = self.binary_operator.get(op_str)
                if op_func is None:
                    raise NotImplementedError(f"Unsupported binary operator {op_str!r} in {tree!r}")
                self.result = op_func(operands[0].value, operands[1].value)
        except (NotImplementedError, ModelicaSemanticError):
            raise
        except Exception as exc:
            raise ModelicaSemanticError(
                f"Unable to evaluate expression: {op_str!r} on {[o.value for o in operands]}"
            ) from exc


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


class _EquationRefResolver(TreeListener):
    """Resolve ComponentRef names in equations to flattened dot-separated names.

    Follows the same depth/cutoff pattern as legacy ComponentRefFlattener:
    when a ComponentRef resolves to a known flat symbol, rewrite it in place;
    when it doesn't (builtins like ``time``, function names, for-loop indices),
    skip it and all its children.
    """

    def __init__(self, flat_class: ast.InstanceClass, prefix: str):
        self.flat_class = flat_class
        self.prefix = prefix
        self.depth = 0
        self.cutoff_depth = sys.maxsize
        super().__init__()

    def reset(self):
        self.depth = 0
        self.cutoff_depth = sys.maxsize

    def enterComponentRef(self, tree: ast.ComponentRef):
        self.depth += 1
        if self.depth > self.cutoff_depth:
            return

        # Compose candidate flat name: prefix.name.child1.child2...
        if self.prefix:
            new_name = self.prefix + "." + tree.name
        else:
            new_name = tree.name
        c = tree
        while len(c.child) > 0:
            assert len(c.child) <= 1
            c = c.child[0]
            new_name += "." + c.name

        if new_name in self.flat_class.symbols:
            tree.name = new_name
            # Merge child indices into parent
            c = tree
            while len(c.child) > 0:
                assert len(c.child) <= 1
                c = c.child[0]
                tree.indices += c.indices
            tree.child = []
        else:
            # Not a known symbol — leave alone (builtin, function, for-index, etc.)
            self.cutoff_depth = self.depth

    def exitComponentRef(self, tree: ast.ComponentRef):
        self.depth -= 1
        if self.depth < self.cutoff_depth:
            self.cutoff_depth = sys.maxsize


class _FunctionCallResolver(TreeListener):
    """Discover function calls in equations/statements, resolve to fully-scoped names."""

    def __init__(self, instance: ast.InstanceClass, functions: OrderedDict):
        self.instance = instance
        self.functions = functions
        super().__init__()

    def exitExpression(self, tree: ast.Expression):
        if not isinstance(tree.operator, ast.ComponentRef):
            return
        try:
            found = _find_name(tree.operator, self.instance)
        except Exception:
            return
        if found is None or not isinstance(found, ast.Class):
            return  # built-in function (sin, cos, etc.) or symbol
        full_name = found.full_name
        tree.operator = full_name
        self.functions[full_name] = found


def _flatten_connect_ref(ref: ast.ComponentRef, prefix: str) -> None:
    """Flatten a connect clause ComponentRef by prepending prefix and collapsing children."""
    parts = [ref.name]
    c = ref
    last_indices = ref.indices
    while len(c.child) > 0:
        assert len(c.child) <= 1
        c = c.child[0]
        parts.append(c.name)
        last_indices = c.indices
    ref.name = prefix + "." + ".".join(parts) if prefix else ".".join(parts)
    ref.indices = last_indices
    ref.child = []


def _is_inner_connector(ref: ast.ComponentRef, instance: ast.InstanceClass) -> bool:
    """Determine if a connect ref is 'inner' (belongs to a sub-component) per MLS 9.2.

    A connector is 'inner' if the first name in the ref resolves to a non-connector
    component (i.e., the connector is a port of that component). It is 'outer' if the
    first name resolves directly to a connector declared in the current class.
    """
    first_name = ref.name
    sym = instance.symbols.get(first_name)
    if sym is None:
        return False
    if (
        isinstance(sym.type, ast.InstanceClass)
        and sym.type.type == "connector"
        and len(ref.child) == 0
    ):
        # Bare connector name in this scope → outer
        return False
    # Component with sub-connector (or deeper path) → inner
    return True


def _collect_and_resolve_equations(
    instance: ast.InstanceClass,
    flat_class: ast.InstanceClass,
    prefix: str,
    functions: Optional[OrderedDict] = None,
) -> None:
    """Deep-copy equations from *instance*, resolve refs, append to *flat_class*."""
    walker = TreeWalker()
    resolver = _EquationRefResolver(flat_class, prefix)
    func_resolver = _FunctionCallResolver(instance, functions) if functions is not None else None

    # Snapshot lists before iteration to avoid mutation issues if the
    # instance's lists are modified during extends processing.
    equations = instance.equations
    initial_equations = instance.initial_equations
    statements = instance.statements
    initial_statements = instance.initial_statements

    for eq in equations:
        eq_copy = copy.deepcopy(eq)
        if isinstance(eq_copy, ast.ConnectClause):
            # Annotate inner/outer per MLS 9.2 (before flattening collapses children)
            eq_copy.__left_inner = _is_inner_connector(eq.left, instance)
            eq_copy.__right_inner = _is_inner_connector(eq.right, instance)
            # Manually flatten connect refs (connector names aren't in flat_class.symbols)
            _flatten_connect_ref(eq_copy.left, prefix)
            _flatten_connect_ref(eq_copy.right, prefix)
        else:
            if func_resolver is not None:
                walker.walk(func_resolver, eq_copy)
            resolver.reset()
            walker.walk(resolver, eq_copy)
        flat_class.equations.append(eq_copy)

    for eq in initial_equations:
        eq_copy = copy.deepcopy(eq)
        if func_resolver is not None:
            walker.walk(func_resolver, eq_copy)
        resolver.reset()
        walker.walk(resolver, eq_copy)
        flat_class.initial_equations.append(eq_copy)

    for stmt in statements:
        stmt_copy = copy.deepcopy(stmt)
        if func_resolver is not None:
            walker.walk(func_resolver, stmt_copy)
        resolver.reset()
        walker.walk(resolver, stmt_copy)
        flat_class.statements.append(stmt_copy)

    for stmt in initial_statements:
        stmt_copy = copy.deepcopy(stmt)
        if func_resolver is not None:
            walker.walk(func_resolver, stmt_copy)
        resolver.reset()
        walker.walk(resolver, stmt_copy)
        flat_class.initial_statements.append(stmt_copy)


def _flatten_discovered_functions(
    functions: OrderedDict,
    flat_class: ast.InstanceClass,
) -> None:
    """Flatten discovered function classes and add to flat_class.functions."""
    processed = set()
    while set(functions.keys()) - processed:
        for full_name in list(functions.keys()):
            if full_name in processed:
                continue
            func_class = functions[full_name]

            # Instantiate if needed
            if (
                not isinstance(func_class, ast.InstanceClass)
                or func_class.instantiation_state < ast.InstantiationState.FULL
            ):
                func_instance = _instantiate_element(
                    func_class,
                    func_class.parent,
                    ast.ClassModification(),
                    current_instances=None,
                )
            else:
                func_instance = func_class

            # Flatten using new pipeline — discovers nested function calls
            nested_functions = OrderedDict()
            flat_func = _create_partial_flat_instance(func_instance)
            _flatten_instance(func_instance, flat_func, functions=nested_functions)

            # Merge nested discoveries back
            for nested_name, nested_class in nested_functions.items():
                if nested_name not in functions:
                    functions[nested_name] = nested_class

            flat_class.functions[full_name] = flat_func
            processed.add(full_name)


def _check_all_references_valid(flat_class: ast.InstanceClass):
    # TODO: 1.9 Check that all references now point to a valid instance (not conditionally false)
    pass


def _generate_connect_equations(flat_class: ast.InstanceClass):
    """Generate equations from connect clauses (MLS 9.2).

    For each ConnectClause, expand into variable-level equations:
    - Non-flow variables (no prefix, input, output): equality equations
    - Flow variables: Kirchhoff sum-to-zero equations
    - Constant/parameter: skipped
    Disconnected flow variables default to 0.
    """
    # Track disconnected flow variables
    disconnected_flow_variables = OrderedDict()
    for name, sym in flat_class.symbols.items():
        if "flow" in sym.prefixes:
            disconnected_flow_variables[name] = name

    flow_connections = OrderedDict()
    orig_equations = flat_class.equations[:]
    flat_class.equations = []

    for equation in orig_equations:
        if not isinstance(equation, ast.ConnectClause):
            flat_class.equations.append(equation)
            continue

        left_inner = getattr(equation, "__left_inner", False)
        right_inner = getattr(equation, "__right_inner", False)
        left_name = equation.left.name
        right_name = equation.right.name

        # Find connector variables by prefix in flat_class.symbols
        left_prefix = left_name + "."
        connector_vars = []
        for sym_name, sym in flat_class.symbols.items():
            if sym_name.startswith(left_prefix):
                suffix = sym_name[len(left_prefix) :]
                type_indices = sym.type.indices if hasattr(sym.type, "indices") else []
                connector_vars.append((suffix, sym.prefixes, type_indices))

        if not connector_vars:
            # Elementary type connection (e.g., connecting Reals directly)
            flat_class.equations.append(ast.Equation(left=equation.left, right=equation.right))
            continue

        for suffix, prefixes, type_indices in connector_vars:
            l_name = left_name + "." + suffix
            r_name = right_name + "." + suffix
            left = ast.ComponentRef(
                name=l_name,
                indices=equation.left.indices + type_indices,
            )
            right = ast.ComponentRef(
                name=r_name,
                indices=equation.right.indices + type_indices,
            )

            if len(prefixes) == 0 or prefixes[0] in ["input", "output"]:
                flat_class.equations.append(ast.Equation(left=left, right=right))
            elif "flow" in prefixes:
                # Build flow connection groups for sum-to-zero equations
                left_key = (
                    l_name,
                    tuple(
                        i.value
                        for index_array in left.indices
                        for i in index_array
                        if i is not None
                    ),
                    left_inner,
                )
                right_key = (
                    r_name,
                    tuple(
                        i.value
                        for index_array in right.indices
                        for i in index_array
                        if i is not None
                    ),
                    right_inner,
                )

                left_connected = flow_connections.get(left_key, OrderedDict())
                right_connected = flow_connections.get(right_key, OrderedDict())

                left_connected.update(right_connected)
                connected = left_connected
                connected[left_key] = (left, left_inner)
                connected[right_key] = (right, right_inner)

                for k in connected:
                    flow_connections[k] = connected

                disconnected_flow_variables.pop(l_name, None)
                disconnected_flow_variables.pop(r_name, None)
            elif prefixes[0] in ["constant", "parameter"]:
                pass
            else:
                raise Exception("Unsupported connector variable prefixes {}".format(prefixes))

    # Generate flow sum-to-zero equations
    processed: Set[int] = set()
    for connected_variables in flow_connections.values():
        if id(connected_variables) not in processed:
            processed.add(id(connected_variables))
            operand_specs = list(connected_variables.values())
            if all(not op_spec[1] for op_spec in operand_specs):
                # All outer variables — no sign inversion needed
                operands = [op_spec[0] for op_spec in operand_specs]
            else:
                operands = [
                    (
                        op_spec[0]
                        if op_spec[1]
                        else ast.Expression(operator="-", operands=[op_spec[0]])
                    )
                    for op_spec in operand_specs
                ]
            expr = operands[-1]
            for op in reversed(operands[:-1]):
                expr = ast.Expression(operator="+", operands=[op, expr])
            flat_class.equations.append(ast.Equation(left=expr, right=ast.Primary(value=0)))

    # Disconnected flow variables default to 0
    for name in disconnected_flow_variables.values():
        flat_class.equations.append(
            ast.Equation(left=ast.ComponentRef(name=name), right=ast.Primary(value=0))
        )


def _process_transitions(flat_class: ast.InstanceClass):
    # TODO: 3. Process transitions in the flattened tree
    pass


def _flatten_name(
    element: ast.InstanceElement,
    remove_prefix: str = "",
) -> str:
    """Flatten the instance path into a name str

    :param instance: The instance having the name to flatten
    :param remove_prefix: The prefix to remove from the name
    :return: The flattened name
    """
    assert isinstance(element, ast.InstanceElement)
    element_full_name = ast.element_instance_name_tuple(element)
    flat_name_start = 0
    for names in zip(element_full_name, remove_prefix.split(".")):
        if names[0] == names[1]:
            flat_name_start += 1
    return ".".join(element_full_name[flat_name_start:])


def _resolve_name(
    name: Union[str, ast.ComponentRef],
    scope: ast.InstanceClass,
    flat_class: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
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

    # When scope is a raw ast.Class (not yet an InstanceClass), find the
    # corresponding InstanceClass by walking up the instance tree from
    # flat_class.  This happens for deeply-nested modifications whose scope
    # wasn't resolved to an InstanceClass during instantiation.
    if not isinstance(scope, ast.InstanceClass):
        current = flat_class
        while current is not None:
            if isinstance(current, ast.InstanceClass) and (
                current.ast_ref is scope or current.ast_ref.full_name == scope.full_name
            ):
                scope = current
                break
            current = getattr(current, "parent_instance", None)
        if not isinstance(scope, ast.InstanceClass):
            raise NameLookupError(
                f"Unable to find instance for scope {scope.full_name} from {flat_class.full_name}"
            )

    # Step C: Fully instantiate the scope class if needed
    if scope.instantiation_state < ast.InstantiationState.FULL:
        scope = _instantiate_class(
            scope,
            ast.ClassModification(),
            scope.parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )

    found = _find_name(
        name,
        scope,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )
    if found is None:
        raise NameLookupError(f"Unable to resolve {name} in scope {scope.full_name}")

    is_symbol = isinstance(found, ast.Symbol)

    if (
        not isinstance(found, ast.InstanceElement)
        or found.instantiation_state < ast.InstantiationState.FULL
    ):
        element = _instantiate_element(
            found,
            scope,
            ast.ClassModification(),
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
    else:
        element = found

    flat_name = _flatten_name(element, flat_class.full_instance_name)

    # Check to see if the name is already resolved
    if is_symbol and flat_name in flat_class.symbols:
        return flat_class.symbols[flat_name]
    elif flat_name in flat_class.classes:
        return flat_class.classes[flat_name]

    # Make copy so we can flatten name without changing originals.
    # Reparent under flat_class so full_instance_name / full_name reflect
    # the flat path (flat_name already encodes the ancestor components).
    element = element.clone()
    element.name = flat_name
    element.parent_instance = flat_class
    element.parent = flat_class
    if is_symbol:
        flat_class.symbols[flat_name] = element
    else:
        flat_class.classes[flat_name] = element

    return element


def _instantiate_element(
    element: Union[ast.Class, ast.Symbol, ast.InstanceClass, ast.InstanceSymbol],
    scope: ast.InstanceClass,
    modification_environment: ast.ClassModification,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]] = None,
    instantiate_in_place: bool = False,
    update_parent_instance: bool = True,
    target_state: ast.InstantiationState = ast.InstantiationState.FULL,
) -> Union[ast.InstanceClass, ast.InstanceSymbol]:

    is_symbol = isinstance(element, ast.Symbol)

    if isinstance(element, ast.InstanceElement):
        if is_symbol:
            # For symbols, class_to_instantiate (below) will be the
            # containing class (element.parent).  Its parent in the instance
            # tree is element.parent_instance.parent_instance, NOT
            # element.parent_instance (which IS the containing class).
            parent_instance = element.parent_instance.parent_instance
        else:
            parent_instance = element.parent_instance
        parent = element.parent
    else:
        element_class = element.parent if is_symbol else element
        parent = _get_lexical_parent_instance(
            element_class,
            scope,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
        parent_instance = parent

    class_to_instantiate = element.parent if is_symbol else element
    instance = _instantiate_class(
        class_to_instantiate,
        ast.ClassModification(),
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
        update_parent_instance=update_parent_instance,
        target_state=target_state,
    )

    if is_symbol:
        return instance.symbols[element.name]
    else:
        return instance


# ---------------------------------------------------------------------------
# Adapter: InstanceClass → ast.Class / ast.Tree  (for backend compatibility)
# ---------------------------------------------------------------------------

_VALUE_ATTRS = (
    "start",
    "min",
    "max",
    "nominal",
    "value",
    "fixed",
    "unit",
    "quantity",
    "displayUnit",
)


def _get_constant_value(isym: ast.InstanceSymbol):
    """Extract the resolved constant value from an InstanceSymbol, or None if unavailable."""
    if "constant" not in isym.prefixes:
        return None
    # Check if value was already resolved
    if isym.value is not None and not (
        isinstance(isym.value, ast.Primary) and isym.value.value is None
    ):
        return isym.value
    # Look in the type class's builtin symbol modification for the value
    if isinstance(isym.type, ast.InstanceClass) and isym.type.name in InstanceTree.BUILTIN_TYPES:
        builtin_sym = isym.type.symbols.get(isym.type.name)
        if builtin_sym is not None:
            for arg in builtin_sym.modification_environment.arguments:
                if (
                    isinstance(arg.value, ast.ElementModification)
                    and arg.value.component.name == "value"
                    and arg.value.modifications
                ):
                    mod_val = arg.value.modifications[0]
                    if isinstance(mod_val, ast.Primary):
                        return mod_val
    return None


def _to_ast_value(val):
    """Wrap a raw Python value or InstanceSymbol back into an AST node."""
    if isinstance(val, ast.Primary):
        return val
    if isinstance(val, (ast.Expression, ast.Array, ast.ComponentRef)):
        return val
    if isinstance(val, ast.InstanceSymbol):
        # For constants, substitute the actual value (MLS 5.6.2)
        const_val = _get_constant_value(val)
        if const_val is not None:
            return const_val
        return ast.ComponentRef(name=val.name)
    if isinstance(val, (int, float, bool, str)) or val is None:
        return ast.Primary(value=val)
    return val


def _instance_to_ast_symbol(isym: ast.InstanceSymbol) -> ast.Symbol:
    """Convert an InstanceSymbol to a plain ast.Symbol for TreeWalker compatibility."""
    sym = ast.Symbol()
    for attr in (
        "name",
        "type",
        "prefixes",
        "replaceable",
        "final",
        "inner",
        "outer",
        "dimensions",
        "comment",
        "id",
        "order",
        "visibility",
        "class_modification",
    ):
        setattr(sym, attr, getattr(isym, attr))
    # Wrap raw values back into ast.Primary for backend compatibility
    for attr in _VALUE_ATTRS:
        setattr(sym, attr, _to_ast_value(getattr(isym, attr)))
    # Normalize type to ComponentRef for TreeWalker compatibility
    if isinstance(sym.type, ast.InstanceClass):
        sym.type = ast.ComponentRef(name=sym.type.name)
    return sym


def _instance_to_ast_class(flat_instance: ast.InstanceClass) -> ast.Class:
    """Convert a flat InstanceClass to a plain ast.Class for backend consumption."""
    flat_class = ast.Class()
    flat_class.name = flat_instance.name
    flat_class.type = flat_instance.type
    flat_class.comment = flat_instance.comment
    flat_class.annotation = flat_instance.annotation

    # Convert symbols
    for name, isym in flat_instance.symbols.items():
        sym = _instance_to_ast_symbol(isym)
        sym.parent = flat_class
        flat_class.symbols[name] = sym

    # Equations are plain AST nodes — copy directly
    flat_class.equations = flat_instance.equations
    flat_class.initial_equations = flat_instance.initial_equations
    flat_class.statements = flat_instance.statements
    flat_class.initial_statements = flat_instance.initial_statements

    # Functions
    for fname, func in flat_instance.functions.items():
        if isinstance(func, ast.InstanceClass):
            flat_class.functions[fname] = _instance_to_ast_class(func)
        else:
            flat_class.functions[fname] = func

    return flat_class


def _add_connector_symbols(
    instance: ast.InstanceClass,
    flat_class: ast.Class,
    prefix: str,
) -> None:
    """Walk the instance tree and add connector-level symbols to *flat_class*.

    ``expand_connectors`` expects a symbol entry for each connector (e.g.
    ``a.up``) with a ``__connector_type`` attribute containing a flat
    ``ast.Class`` of the connector's variables.  The new flattening only
    produces leaf symbols (``a.up.H``, ``a.up.Q``), so we recreate the
    intermediate entries here.
    """
    for name, sym in instance.symbols.items():
        if not isinstance(sym.type, ast.InstanceClass):
            continue
        full_name = prefix + "." + name if prefix else name
        if sym.type.type == "connector":
            # Create a stub symbol for this connector
            stub = ast.Symbol()
            stub.name = full_name
            stub.type = ast.ComponentRef(name=sym.type.name)
            stub.prefixes = list(sym.prefixes)
            stub.comment = sym.comment
            # Flatten the connector type to get its variables
            connector_flat = flatten_instance(sym.type)
            stub.__connector_type = _instance_to_ast_class(connector_flat)
            flat_class.symbols[full_name] = stub
        # Recurse into composite types (models containing connectors)
        _add_connector_symbols(sym.type, flat_class, full_name)


def flatten_to_tree(root: ast.Tree, class_name: ast.ComponentRef) -> ast.Tree:
    """Flatten a class using the new instantiation/flattening pipeline,
    returning an ast.Tree compatible with legacy backends.

    Plug-compatible with legacy ``flatten(root, class_name)``.
    """
    from ._legacy import (
        add_state_value_equations,
        add_variable_value_statements,
        annotate_states,
        expand_connectors,
        flatten_component_refs,
    )

    # 1. Instantiate
    class_name_str = str(class_name)
    instance = instantiate(class_name_str, root)

    # 2. Flatten (keep connectors for expand_connectors)
    flat_instance = flatten_instance(instance, keep_connectors=True)

    # 3. Convert InstanceClass → ast.Class
    flat_class = _instance_to_ast_class(flat_instance)

    # 4. Add connector-level symbols with __connector_type for expand_connectors.
    #    The new flattening recurses into connectors producing leaf symbols (e.g.
    #    a.up.H, a.up.Q) but expand_connectors expects an intermediate symbol
    #    for each connector (e.g. a.up) with __connector_type set.
    _add_connector_symbols(instance, flat_class, prefix="")

    # 5. Resolve component refs in symbol attributes (e.g. value = 2 * p1 → 2 * nested.p1)
    for sym_name, sym in list(flat_class.symbols.items()):
        # Derive the instance prefix from the flat symbol name
        if "." in sym_name:
            prefix = sym_name.rsplit(".", 1)[0] + "."
        else:
            prefix = ""
        flat_sym = flatten_component_refs(flat_class, sym, prefix)
        flat_class.symbols[sym_name] = flat_sym

    # 6. Post-processing (same order as legacy flatten)
    expand_connectors(flat_class)
    add_state_value_equations(flat_class)
    for func in flat_class.functions.values():
        add_variable_value_statements(func)
    annotate_states(flat_class)

    # 6. Build ast.Tree
    out = ast.Tree()
    flat_name = class_name_str
    flat_class.name = flat_name
    out.classes[flat_name] = flat_class

    # Pull functions to top level (before the model class)
    functions_and_classes = flat_class.functions
    flat_class.functions = OrderedDict()
    functions_and_classes.update(out.classes)
    out.classes = functions_and_classes

    return out
