#!/usr/bin/env python
"""
Modelica instantiation — MLS 5.6.1

Entry: instantiate(class_name, class_tree) → _instantiate_class()
"""

from __future__ import absolute_import, division, print_function, unicode_literals

import copy  # TODO
from collections import OrderedDict
from typing import List, Optional, Set, Union

from ._base import InstantiationError, ModelicaSemanticError, NameLookupError
from ._name_lookup import _find_name, find_name
from .. import ast


class InstanceTree(ast.Tree):
    """The root class of an instance tree

    :param ast_ref: The root of the `ast.Tree` produced by the parser

    The InstanceTree is the unnamed root of the instance tree containing classes
    instantiated from the `ast.Tree` produced by the parser. Built-in types, functions,
    and operators are to be added to the root of the InstanceTree when it is created so
    they can be found during name lookup.
    """

    def __init__(self, ast_ref: ast.Tree, **kwargs):
        # The Class AST
        self.ast_ref = ast_ref

        super().__init__(**kwargs)
        self._instantiate_builtins()

    def _instantiate_builtins(self):
        """Add instantiated built-in types to the instance tree"""
        # TODO: Add built-in functions and operators
        builtin_instances = OrderedDict()
        for name, class_ in self.classes.items():
            builtin_instances[name] = _instantiate_class(class_, ast.ClassModification(), self)
        self.classes.update(builtin_instances)


def instantiate(class_name: str, class_tree: ast.Tree) -> ast.InstanceClass:
    """Instantiate a class from the class tree

    :param class_name: The name of the class to instantiate
    :param class_tree: The AST tree containing class definitions
    :return: Instantiated class in an instance tree ready for flattening

    The returned class will be fully instantiated, but name lookup
    in the instance tree may return `ast.Class` or
    `ast.InstanceClass` with the `fully_instantiated` flag set to
    `False`. If so, these cases will need to be fully instantiated
    for flattening by calling this function on the class.
    """
    instance_tree = InstanceTree(class_tree)
    class_ = find_name(class_name, instance_tree)
    if class_ is None:
        raise NameLookupError(f"{class_name} not found in given tree")
    if isinstance(class_, ast.Symbol):
        raise InstantiationError(f"Found Symbol for {class_name} but need Class to instantiate")
    # Spec v 3.5 section 5.6.1.3 says the instance tree root is parent in the top-level call
    # That section also says the instance should be stored in the parent instance
    # so _instantiate_class does this
    instance = _instantiate_class(class_, ast.ClassModification(), instance_tree)
    return instance


def _instantiate_class(
    orig_class: Union[ast.Class, ast.InstanceClass],
    modification_environment: ast.ClassModification,
    parent_instance: Union["InstanceTree", ast.InstanceClass],
    current_instances: Optional[Set[ast.InstanceClass]] = None,
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]] = None,
    instantiate_in_place: bool = False,
    update_parent_instance: bool = True,
    partially: bool = False,
) -> ast.InstanceClass:
    """Instantiate a class

    :param orig_class: The class to be instantiated
    :param modification_environment: The modification environment of the class
        instance
    :param parent_instance: The parent class this class is contained in
    :param current_instances: instances in process of being instantiated (at least partially)
    :param current_extends: `extends` in process of name lookup (see _find_inherited)
    :param instantiate_in_place: If True, partially instantiate for name lookup if not already
    :param update_parent_instance: If True, the parent instance will be updated with the instantiated class
    :return: The instantiated class

    Implements the instantiation rules per Modelica Language Specification version
    3.5 section 5.6.1.
    """

    # Outline of spec 3.5 section 5.6.1 *Instantiation*:
    # Definitions
    #   - Element: Class, Component (Symbol in Pymoca), or Extends Clause
    # 1. For element itself:
    #   1. Create an instance of the element to be instantiated ("partially instantiated element")
    #   2. Modifiers are merged for the element itself (but contained references are resolved during flattening)
    #   3. Redeclare of element itself is done
    # 2. For each element (Class or Component) in the local contents of the current element:
    #   1. Apply step 1 to the element
    #   2. Equations, algorithms, and annotations are copied into the component instance without merging
    #      (but references in equations are resolved later during flattening)
    # 3. For each element in the extends clauses of the current element:
    #   1. Apply steps 1 and 2 to the element, replacing the extends clause with the extends instance
    # 4. Lookup classes of extends and ensure it is identical to lookup result from step 3
    # 5. Check that all children of the current element (including extends) with same name are identical
    #    (error if not) and only keep one if so (to preserve function argument order)
    # 6. Components are recursively instantiated

    lexical_parent = _get_lexical_parent_instance(
        orig_class,
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )

    # 1.1. Partially instantiate the element itself and 1.2 merge modifiers
    if (
        not isinstance(orig_class, ast.InstanceClass)
        or orig_class.name not in parent_instance.classes
        or modification_environment.arguments
    ):
        new_class = _instantiate_partially(
            orig_class,
            modification_environment,
            parent_instance,
            lexical_parent,
            update_parent_instance=update_parent_instance,
        )
    else:
        # Class already at least partially instantiated and no modifications
        new_class = parent_instance.classes[orig_class.name]
        # FIXME: Delete if not used
        # # Merge element modifications into environment and apply
        # modification_environment.arguments = (
        #     orig_class.modification_environment.arguments + modification_environment.arguments
        # )
        # if modification_environment.arguments:
        #     new_class = new_class.clone()
        #     _apply_modifications(new_class, modification_environment)
        if new_class.fully_instantiated or partially and new_class.partially_instantiated:
            return new_class

    if current_instances is None:
        current_instances = set()
    current_instances.add(new_class)

    # 1.3. Redeclare of element itself is done
    redeclared = _apply_redeclares(
        new_class,
        modification_environment,
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )
    if redeclared:
        return new_class

    # 2.1 Partially instantiate local classes, symbols, and extends
    if (
        isinstance(orig_class, ast.InstanceClass)
        and not orig_class.fully_instantiated
        or orig_class.name in InstanceTree.BUILTIN_TYPES
    ):
        from_class = new_class.ast_ref
    else:
        from_class = orig_class

    # Maintain lexical instance tree for the new class
    if new_class.ast_ref.name in lexical_parent.classes:
        lexical_parent = lexical_parent.classes[new_class.ast_ref.name]
    else:
        lexical_parent = _instantiate_partially(
            from_class,
            ast.ClassModification(),
            lexical_parent,
            lexical_parent,
        )
        assert isinstance(lexical_parent, (ast.InstanceClass, InstanceTree))

    if not new_class.partially_instantiated:

        for class_ in from_class.classes.values():
            _instantiate_partially(
                class_,
                modification_environment,
                new_class,
                lexical_parent,
            )

        for symbol in from_class.symbols.values():
            if not isinstance(symbol, ast.InstanceSymbol):
                symbol = _update_class_modification_scopes(symbol, new_class)
            _instantiate_partially(
                symbol,
                modification_environment,
                new_class,
                lexical_parent,
                class_modification=symbol.class_modification,
            )

        new_class.extends = _instantiate_extends_list(
            from_class.extends,
            modification_environment,
            new_class,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            partially=True,
        )

    new_class.partially_instantiated = True
    current_instances.remove(new_class)
    if partially:
        return new_class

    # 2.2 Copy local contents into the element itself
    _copy_class_contents(new_class, copy_extends=False)

    # We changed step 3. Instantiate extends to partial instantiation including contents

    # TODO: Step 4. Check extends class lookup
    # TODO: Step 5: Check and cull elements with same name in _instantiate_class
    # See `parse_test.test_instantiation_function_input_order`

    # 6. Recursively instantiate symbols (local and inherited)
    for symbol in new_class.symbols.values():
        _instantiate_symbol(
            symbol,
            new_class,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )

    # Recurse down through all levels of extends and instantiate symbols
    _instantiate_extends_symbols(
        new_class.extends,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )

    new_class.fully_instantiated = True

    return new_class


def _instantiate_extends_symbols(
    extends: List[ast.InstanceClass],
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
):
    for extend_node in extends:
        for symbol in extend_node.symbols.values():
            _instantiate_symbol(
                symbol,
                extend_node,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )
        if extend_node.extends:
            _instantiate_extends_symbols(
                extend_node.extends,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )


def _update_class_modification_scopes(
    element: Union[ast.Symbol, ast.ExtendsClause],
    current_scope: ast.InstanceClass,
) -> Union[ast.Symbol, ast.ExtendsClause]:
    """Update the scopes of modification arguments to the given scope if not already done"""

    def resolve_scope(old_scope, new_scope):
        scope = new_scope
        if new_scope.ast_ref.full_name != old_scope.full_name:
            # Must be a short class definition which does not create a new scope
            # So we go up one level to the parent instance of the parent instance
            scope = scope.parent
        return scope

    scoped_mod_args = []
    for arg in element.class_modification.arguments:
        if isinstance(arg.scope, ast.InstanceClass):
            # Already done
            scoped_mod_args.append(arg)
            continue
        # FIXME: Remove commented out code
        # scope = current_scope
        # if scope.ast_ref.full_name != arg.scope.full_name:
        #     # Must be a short class definition which does not create a new scope
        #     # So we go up one level to the parent instance of the parent instance
        #     scope = scope.parent_instance
        scope = resolve_scope(arg.scope, current_scope)
        # Make a copy so we don't change original AST or same arg used elsewhere
        new_arg = copy.copy(arg)
        new_arg.scope = scope
        scoped_mod_args.append(new_arg)
    if scoped_mod_args or isinstance(element, ast.ExtendsClause):
        element = copy.copy(element)
        element.class_modification = ast.ClassModification(arguments=scoped_mod_args)
    if isinstance(element, ast.ExtendsClause):
        element.scope = resolve_scope(element.scope, current_scope)
    return element


def _check_extends_rules(extends_list: List[ast.InstanceClass], class_: ast.InstanceClass) -> None:
    """Check the extends rules over the full extends list"""

    # Check we do not extend from any symbols/classes inherited
    extends_names = {}
    for extends in extends_list:
        extends_names[extends.ast_ref.name] = extends.ast_ref.full_name

    extends_builtin = set()
    extends_other = set()
    for extends_name, extends_component_ref in extends_names.items():
        if extends_name in InstanceTree.BUILTIN_TYPES:
            extends_builtin.add(extends_name)
            # Built-in classes contain a symbol with the same name. This causes an error
            # in the check below, so skip them. Built-in names are checked after this
            # loop completes.
            continue
        extends_other.add(extends_name)
        for other_class in extends_list:
            other_names = {
                *other_class.ast_ref.symbols.keys(),
                *other_class.ast_ref.classes.keys(),
            }
            if extends_name in other_names:
                raise ModelicaSemanticError(
                    f"Cannot extend '{class_.full_name}' with '{extends_component_ref}'; "
                    f"'{extends_name}' also exists in names inherited from '{other_class.ast_ref.name}'"
                )
    if len(extends_builtin) > 1 or len(extends_builtin) and len(extends_other):
        raise ModelicaSemanticError(
            "When extending a built-in class (Real, Integer, ...) you cannot extend other classes as well"
        )


def _instantiate_extends_list_partially(
    extends_list: List[Union[ast.ExtendsClause, ast.Class, ast.InstanceClass]],
    modification_environment: ast.ClassModification,
    parent_instance: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> List[ast.InstanceClass]:
    """Instantiate extends list of clause, class, or instance"""

    partially_instantiated_extends = []

    for extends in extends_list:
        assert isinstance(extends, (ast.ExtendsClause, ast.Class))
        extends_instance = _instantiate_extends_partially(
            extends,
            modification_environment,
            parent_instance,
            current_instances,
            current_extends,
            instantiate_in_place,
        )
        partially_instantiated_extends.append(extends_instance)

    _check_extends_rules(partially_instantiated_extends, parent_instance)

    return partially_instantiated_extends


def _instantiate_extends_partially(
    extends: Union[ast.ExtendsClause, ast.Class, ast.InstanceClass],
    modification_environment: ast.ClassModification,
    parent_instance: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> ast.InstanceClass:
    """Instantiate an extends clause partially"""

    class_modification = ast.ClassModification()
    if isinstance(extends, ast.ExtendsClause):
        extends = _update_class_modification_scopes(extends, parent_instance)
        extends_class = _find_extends_class(
            extends.component,
            extends.scope,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
        class_modification.arguments = (
            extends.class_modification.arguments
            + parent_instance.modification_environment.arguments
        )
    else:
        extends_class = extends
        class_modification.arguments = copy.copy(parent_instance.modification_environment.arguments)

    # When the extends class is an InstanceElement (e.g., from name lookup of A.D),
    # its modification_environment contains mods from the base class definition
    # (inner/middle level). These must come BEFORE the extends clause mods (outer)
    # in class_modification to maintain correct inner-to-outer ordering.
    # We skip the normal element merge in _instantiate_partially to prevent these
    # mods from being placed in the shared modification_environment separately,
    # which would cause them to be re-merged in the wrong order during the full pass.
    skip_element_merge = False
    if (
        isinstance(extends_class, ast.InstanceElement)
        and extends_class.modification_environment.arguments
    ):
        class_modification.arguments = (
            extends_class.modification_environment.arguments + class_modification.arguments
        )
        skip_element_merge = True

    lexical_parent = _get_lexical_parent_instance(
        extends_class,
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )
    extends_class = _instantiate_partially(
        extends_class,
        modification_environment,
        parent_instance,
        lexical_parent,
        update_parent_instance=False,
        class_modification=class_modification,
        skip_element_merge=skip_element_merge,
    )

    # Unnamed node (per spec)
    extends_class.name = ""

    return extends_class


def _instantiate_extends_list(
    extends_list: List[Union[ast.ExtendsClause, ast.Class, ast.InstanceClass]],
    modification_environment: ast.ClassModification,
    parent_instance: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    partially: bool = False,
) -> List[ast.InstanceClass]:
    """Instantiate extends in given list either partially for name lookup or fully"""

    # Nothing to do, just return instead of wasting time below
    if not extends_list:
        return []

    # We make 2 passes on the extends list due to potential name lookup dependencies
    extends_partially_instantiated = []
    for extends in extends_list:
        extends_instance = _instantiate_extends_partially(
            extends,
            modification_environment,
            parent_instance,
            current_instances,
            current_extends,
            instantiate_in_place,
        )
        extends_partially_instantiated.append(extends_instance)

    _check_extends_rules(extends_partially_instantiated, parent_instance)

    extends_list_instantiated = []
    for extends in extends_partially_instantiated:
        extends_instance = _instantiate_class(
            extends,
            modification_environment,
            parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            update_parent_instance=False,
            partially=partially,
        )
        extends_list_instantiated.append(extends_instance)

    return extends_list_instantiated


def _find_extends_class(
    extends_name: Union[str, ast.ComponentRef],
    scope: Union["InstanceTree", ast.InstanceClass],
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> Union[ast.Class, ast.InstanceClass]:
    """Find the extends class and do checks"""

    extends_class = _find_name(
        extends_name,
        scope,
        search_inherited=False,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )

    if extends_class is None:
        raise ModelicaSemanticError(
            f"Extends name {extends_name} not found in scope {scope.full_name}"
        )
    if isinstance(extends_class, ast.Symbol):
        raise ModelicaSemanticError(
            f"Cannot extend a component: {extends_name} in {scope.full_name}"
        )
    if extends_class.full_name == scope.full_name:
        raise ModelicaSemanticError(f"Cannot extend class '{extends_class.full_name}' with itself")
    if _is_transitively_replaceable(extends_class):
        comp = extends_class.name
        full_name = extends_class.parent.full_name
        raise ModelicaSemanticError(
            f"In {full_name} extends {comp}, {comp} and parents cannot be replaceable"
        )

    return extends_class


def _get_lexical_parent_instance(
    class_: Union[ast.Class, ast.InstanceClass],
    lookup_scope: Union[ast.Class, ast.InstanceClass, "InstanceTree"],
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> Union[ast.InstanceClass, "InstanceTree"]:
    """Find the lexical parent instance of the given class"""

    if isinstance(class_, ast.InstanceClass):
        return class_.parent
    if not isinstance(lookup_scope, ast.InstanceClass):
        # Scope is an ast.Class, so the class parent is the lexical parent
        found = class_.parent
    else:
        # Lookup class in the instance tree
        found = _find_name(
            class_.name,
            lookup_scope,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=False,
        )
    if (
        found is not None
        and isinstance(found.parent, ast.InstanceClass)
        and found.full_name == class_.full_name
    ):
        return found.parent
    # Instance parent not found, so create a branch with parent instances of the class
    ancestors = class_.ancestors()
    parent = lookup_scope.root
    if not isinstance(parent, InstanceTree):
        parent = InstanceTree(parent)
    modifications = ast.ClassModification()
    for cls in ancestors[1:]:
        # Guard against stomping on a name in root
        update_parent = cls.name not in parent.classes
        parent = _instantiate_partially(cls, modifications, parent, parent, update_parent)
        modifications = parent.modification_environment
    return parent


def _is_transitively_replaceable(class_: ast.Class) -> bool:
    while True:
        if class_.replaceable:
            return True
        class_ = class_.parent
        if isinstance(class_, ast.Tree):
            break
    return False


def _instantiate_symbol(
    symbol: ast.InstanceSymbol,
    parent_instance: ast.InstanceClass,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> None:
    """Instantiate given symbol"""

    assert isinstance(symbol, ast.InstanceSymbol)
    assert isinstance(parent_instance, ast.InstanceClass)

    if symbol.name in InstanceTree.BUILTIN_TYPES:
        symbol.fully_instantiated = True
        return

    modification_environment = symbol.modification_environment
    _apply_redeclares(
        symbol,
        modification_environment,
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )

    if not isinstance(symbol.type, ast.InstanceClass):
        symbol_type = _find_name(
            symbol.type,
            parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
        if symbol_type is None:
            raise NameLookupError(
                f"Type {symbol.type} of symbol {symbol.name} "
                f"not found in {parent_instance.full_name}"
            )
    else:
        symbol_type = symbol.type

    # FIXME: Delete commented out code if not needed
    # if symbol_type.replaceable != symbol.replaceable:
    #     symbol_type = copy.copy(symbol_type)
    #     symbol_type.replaceable = symbol.replaceable
    symbol.type = _instantiate_class(
        symbol_type,
        modification_environment,
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
        update_parent_instance=False,
    )
    _copy_symbol_contents(symbol)

    symbol.fully_instantiated = True


def _instantiate_partially(
    element: Union[
        ast.Class,
        ast.Symbol,
        ast.InstanceClass,
        ast.InstanceSymbol,
    ],
    modification_environment: ast.ClassModification,
    parent_instance: Union["InstanceTree", ast.InstanceClass],
    parent: Union["InstanceTree", ast.InstanceClass],
    update_parent_instance: bool = True,
    class_modification: Optional[ast.ClassModification] = None,
    skip_element_merge: bool = False,
) -> Union[ast.InstanceClass, ast.InstanceSymbol]:
    """Partially instantiate a class or symbol, apply modifiers, and set visibility"""

    #  Create an instance of the class to be instantiated ("partially instantiated element")
    if isinstance(element, ast.InstanceElement):
        ast_ref = element.ast_ref
        # Merge element modifications into environment
        if not skip_element_merge:
            modification_environment.arguments = (
                element.modification_environment.arguments + modification_environment.arguments
            )
    else:
        ast_ref = element

    # Create the instance and copy attributes needed in name lookup or modification
    if isinstance(element, ast.Class):
        instance = ast.InstanceClass(
            name=element.name,
            ast_ref=ast_ref,
            parent_instance=parent_instance,
            parent=parent,
            annotation=ast.ClassModification(),
            replaceable=element.replaceable,
            encapsulated=element.encapsulated,
            partial=element.partial,
            final=element.final,
        )
        if update_parent_instance:
            parent_instance.classes[element.name] = instance
    else:
        instance = ast.InstanceSymbol(
            name=element.name,
            ast_ref=ast_ref,
            parent_instance=parent_instance,
            parent=parent,
            replaceable=element.replaceable,
            final=element.final,
        )
        if update_parent_instance:
            parent_instance.symbols[element.name] = instance

    # Merge visibility
    instance.visibility = min(ast_ref.visibility, parent.visibility)

    # Modifiers are merged for the element itself
    _apply_modifications(instance, modification_environment, class_modification)

    return instance


def _apply_modifications(
    instance: Union[ast.InstanceClass, ast.InstanceSymbol],
    modification_environment: ast.ClassModification,
    class_modification: Optional[ast.ClassModification] = None,
) -> None:
    """
    Apply modifications to an instance

    The modifications are given in `modification_environment` and an optional `class_modification`.

    Modifications are applied in reverse priority, meaning the last modification in the list
    overrides previous ones. This function handles merging modifiers, shifting them down to
    child components, and applying value modifications.

    Args:
        instance: The instance to which modifications are applied (previously created from element arg)
        modification_environment: The modification environment containing the modifiers
        class_modification: `class_modification`s to add, if any
    Raises:
        ModelicaSemanticError: If subscripting modifiers, which is not allowed
        NotImplementedError: If an unsupported modification type is encountered
    """
    # TODO: Would on-the-fly culling modifiers of same attribute be more efficient?

    # Find args from given modification_environment that apply to this instance
    apply_mod_args = [
        arg
        for arg in modification_environment.arguments
        if isinstance(arg.value, ast.ComponentClause)
        and instance.name in (arg.value.type.name, arg.value.symbol_list[0].name)
        or isinstance(arg.value, ast.ElementModification)
        and instance.name == arg.value.component.name
        or isinstance(arg.value, ast.ShortClassDefinition)
        and instance.name == arg.value.name
        or isinstance(instance, ast.Symbol)
        and instance.name in InstanceTree.BUILTIN_TYPES
    ]

    # Remove from given modification_environment and add to instance
    if apply_mod_args:
        modification_environment.arguments = [
            arg for arg in modification_environment.arguments if arg not in apply_mod_args
        ]
        instance.modification_environment.arguments += apply_mod_args

    # All given class_modification arguments are applied
    mod = ast.ClassModification()
    if class_modification is not None:
        mod.arguments += class_modification.arguments

    # Shift modifiers down
    for arg in instance.modification_environment.arguments:
        if isinstance(arg.value, ast.ElementModification):
            if arg.value.component.indices != [[None]]:
                raise ModelicaSemanticError("Subscripting modifiers is not allowed.")
            if arg.value.component.child:
                # Move component reference down a level and apply modification
                # Don't stomp on original that may be used elsewhere
                arg = copy.copy(arg)
                arg.value = copy.copy(arg.value)
                arg.value.component = arg.value.component.child[0]
                mod.arguments.append(arg)
            else:
                if instance.ast_ref.name in InstanceTree.BUILTIN_TYPES:
                    mod.arguments.append(arg)
                else:
                    for sub_arg in arg.value.modifications:
                        if isinstance(sub_arg, ast.ClassModification):
                            mod.arguments += sub_arg.arguments
                        else:
                            # Value modification - apply to symbol
                            # Don't stomp on original that may be used elsewhere
                            sub_arg = copy.copy(arg)
                            sub_arg.value = copy.copy(arg.value)
                            sub_arg.value.component = ast.ComponentRef(name="value")
                            mod.arguments.append(sub_arg)
        elif isinstance(arg.value, (ast.ShortClassDefinition, ast.ComponentClause)):
            # Redeclares are handled separately
            mod.arguments.append(arg)
        else:
            raise NotImplementedError(f"{arg.value.__class__} modification")

    instance.modification_environment = mod


def _append_modifications(*mods: ast.ClassModification) -> ast.ClassModification:
    """Append modifications in order given"""
    combined_modification = ast.ClassModification()
    for mod in mods:
        combined_modification.arguments += mod.arguments
    return combined_modification


def _apply_redeclares(
    element: Union[ast.InstanceClass, ast.InstanceSymbol],
    modification_environment: ast.ClassModification,
    parent_instance: Union[ast.InstanceClass, "InstanceTree"],
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    partially: bool = False,
) -> bool:
    """Apply redeclare if any and remove from environment

    Modifies the element with the redeclare added as an unnamed extends node.

    :return: True if redeclared."""

    # TODO: Reduce the number of if statements and isinstance checks (separate class and symbol)?
    redeclares = []
    for arg in element.modification_environment.arguments:
        if not arg.redeclare:
            continue
        if (
            isinstance(arg.value, ast.ShortClassDefinition)
            and arg.value.name == element.name
            or isinstance(arg.value, ast.ComponentClause)
            and arg.value.symbol_list[0].name == element.name
        ):
            redeclares.append(arg)
    if not redeclares:
        return False

    if not element.replaceable:
        raise ModelicaSemanticError(f"Redeclaring {element.full_name} that is not replaceable")
    if element.final:
        raise ModelicaSemanticError(f"Redeclaring {element.full_name} that is final is not allowed")
    if isinstance(element, ast.Symbol) and "constant" in element.prefixes:
        raise ModelicaSemanticError(
            f"Redeclaring {element.full_name} that is constant is not allowed"
        )

    # TODO: Disallow redeclaring protected as public or public as protected
    # TODO: Check type sub-typing rules (section 6.4) w.r.t. array dimensions

    # Remove from passed modification_environment
    element.modification_environment.arguments = [
        arg for arg in element.modification_environment.arguments if arg not in redeclares
    ]
    # "Last wins" — the last redeclare in the list is the outermost.
    # The modification merging flow in _instantiate_extends_partially ensures
    # that inner mods come before outer mods in the list.
    apply_redeclare = redeclares[-1]
    redeclare = apply_redeclare.value
    scope_class = apply_redeclare.scope
    assert scope_class, "Redeclare scope should have been set by now"
    if isinstance(redeclare, ast.ShortClassDefinition):
        redeclare_name = redeclare.component
    else:  # ast.ComponentClause
        redeclare_name = redeclare.type.name

    redeclare_class = _find_name(
        redeclare_name,
        scope_class,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
    )

    if redeclare_class is None:
        raise NameLookupError(
            f"Redeclare class {redeclare_name} not found in scope {scope_class.full_name}"
        )
    if isinstance(redeclare_class, ast.Symbol):
        if_symbol_msg = " type" if isinstance(element, ast.Symbol) else ""
        raise ModelicaSemanticError(
            f"Redeclaring {element.name}{if_symbol_msg} with component {redeclare_name}"
            f" in scope {scope_class.full_name}"
        )
    # FIXME: Check/preserve prefixes (section 7.3)
    # FIXME: Check type compatibility, constraining type

    if isinstance(redeclare, ast.ShortClassDefinition):
        apply_args = redeclare.class_modification.arguments
    else:  # ast.ComponentClause
        apply_args = redeclare.symbol_list[0].class_modification.arguments
    modification_environment.arguments = modification_environment.arguments + apply_args

    # Instantiate as an extends and add to extends list
    is_class = isinstance(element, ast.InstanceClass)
    parent_instance = element if is_class else element.parent_instance
    extends_list = [redeclare_class]
    extends_list_instantiated = _instantiate_extends_list(
        extends_list,
        modification_environment,
        parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
        partially=partially,
    )
    redeclare_class = extends_list_instantiated[0]
    redeclare_class.replaceable = redeclare.replaceable
    if isinstance(element, ast.InstanceClass):
        redeclared = element
    # Next cases are ast.InstanceSymbol
    elif isinstance(element.type, ast.InstanceClass):
        redeclared = element.type
    else:
        # element.type is an ast.ComponentRef
        redeclared = _find_name(
            element.type,
            element.parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
        if redeclared is None:
            raise ModelicaSemanticError(
                f"Redeclared type {element.type} not found in {scope_class}"
            )
        redeclared = _instantiate_class(
            redeclared,
            modification_environment,
            element.parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            partially=partially,
        )
        element.type = redeclared

    element.replaceable = redeclare.replaceable
    redeclared.replaceable = redeclare.replaceable
    redeclared.extends.insert(0, redeclare_class)

    return True


def _copy_class_contents(
    to_class: ast.InstanceClass,
    copy_extends=True,
) -> None:
    """Shallow copy of references from original to new class"""
    from_class = to_class.ast_ref
    to_class.imports.update(from_class.imports)
    if copy_extends:
        to_class.extends += from_class.extends
    to_class.equations += from_class.equations
    to_class.initial_equations += from_class.initial_equations
    to_class.statements += from_class.statements
    to_class.initial_statements += from_class.initial_statements
    if isinstance(from_class.annotation, ast.ClassModification):
        to_class.annotation.arguments += from_class.annotation.arguments
    to_class.functions.update(from_class.functions)
    to_class.comment = from_class.comment


def _copy_symbol_contents(to_symbol: ast.InstanceSymbol) -> None:
    """Shallow copy of references from original to new symbol"""
    from_symbol = to_symbol.ast_ref
    for attr_name in (
        name
        for name in from_symbol.__dict__
        if name
        not in (
            "name",
            "type",
            "visibility",
            "ATTRIBUTES",
            "class_modification",
            "parent",
            "replaceable",
        )
    ):
        setattr(to_symbol, attr_name, getattr(from_symbol, attr_name))
