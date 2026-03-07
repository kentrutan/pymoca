#!/usr/bin/env python
"""
Modelica name lookup — MLS Chapter 5

Entry: find_name(name, scope) → _find_name() → _find_simple_name() / _find_rest_of_name()
"""

from __future__ import absolute_import, division, print_function, unicode_literals

import os
from typing import Optional, Set, Tuple, Union

from ._base import NameLookupError
from .. import ast


def find_name(
    name: Union[str, ast.ComponentRef],
    scope: ast.Class,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Modelica name lookup on a tree of ast.Class and ast.InstanceClass starting at scope class

    :param name: name to look up (can be a Class or Symbol name)
    :param scope: scope in which to start name lookup

    Implements lookup rules per Modelica Language Specification version 3.5 chapter 5,
    see also chapter 13. This is more succinctly outlined in the "Modelica by Example"
    book https://mbe.modelica.university/components/packages/lookup/
    """
    if not isinstance(name, (str, ast.ComponentRef)):
        raise TypeError(f"name must be a string or ComponentRef, not {type(name)}")
    if not isinstance(scope, ast.Class):
        raise TypeError(f"scope must be a Class or Tree, not {type(scope)}")
    return _find_name(name, scope)


def _find_name(
    name: Union[str, ast.ComponentRef],
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]] = None,
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]] = None,
    instantiate_in_place: bool = True,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Internal start point for name lookup with extra parameters to control the lookup"""
    # Look for ast.Class or ast.Symbol per the MLS v3.5:
    # 1. Simple Name Lookup (spec 5.3.1)
    #     1. Iteration variables
    #     2. Classes
    #     3. Components (Symbols in Pymoca)
    #     4. Classes and Components from Extends Clauses
    #     5. Qualified Import names, see 4 (but not from Extends Clauses) (spec 13.2.1)
    #     6. Public Unqualified Imports (error if multiple are found) (spec 13.2.1)
    #     7. a) Repeat 1-6 for each lexically enclosing instance scope,
    #        b) stopping at `encapsulated`
    #        c) unless predefined type, function, operator then look in root.
    #        d) If name matches a variable (a.k.a. component, symbol) in an enclosing class, it
    #           must be a `constant`.
    # 2. Composite Name Lookup (e.g. `A.B.C`) (spec 5.3.2)
    #     1. `A` is looked up using Simple Name Lookup
    #     2. If `A` is a Component:
    #         1. `B.C` is looked up from named component elements of `A`
    #         2. If not found and if `A.B.C` is used as a function call and `A` is a
    #            scalar or can be
    #         evaluated as a scalar from an array and `B` and `C` are classes, it is a
    #         non-operator function call.
    #     3. If `A` is a Class:
    #         1. `A` is temporarily flattened without modifiers of this class
    #         2. `B.C` is looked up among named elements of temp flattened class,
    #         but if `A` is not a package, lookup is restricted to `encapsulated` elements
    #         only and "the class we look inside shall not be partial in a simulation
    #         model".
    # 3. Global Name Lookup (e.g. `.A.B.C`) (spec 5.3.3)
    #     1. `A` is looked up in global scope. (`A` must be a class or a global constant.)
    #     2. If `A` is a class, follow procedure 2.3.
    # 4. Imported Name Lookup (e.g. `A.B.C`, `D = A.B.C`, `A.B.*`, or `A.B.{C,D}`) (spec
    #    13.2.1)
    #     1. `A` is looked up in global scope
    #     2. `B.C` (and `B.D`) or `B.*` is looked up. `A.B` must be a package.
    # TODO: Global name lookup

    left_name, rest_of_name = _parse_str_or_ref(name)

    # Lookup simple name first (the `A` part)
    found = _find_simple_name(
        left_name,
        scope,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
        search_imports=search_imports,
        search_parent=search_parent,
        search_inherited=search_inherited,
        check_encapsulated=check_encapsulated,
    )

    # Lookup rest of name (e.g. `B.C`) to complete composite name lookup
    # Per MLS 5.6.1, search_inherited=False only restricts the first (simple) name
    # lookup in extends clauses. Once the first name is found, the rest of the
    # composite name should use normal lookup including inherited elements.
    if found is not None and rest_of_name:
        found = _find_rest_of_name(
            found,
            rest_of_name,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            search_imports=search_imports,
            search_parent=search_parent,
            search_inherited=True,
            check_encapsulated=check_encapsulated,
        )

    # Maintaining backward compatibility by including InstanceTree (not strictly correct)
    # TODO: Remove InstanceTree to make spec compliant and fix test/models
    # Late import to avoid circular dependency
    from ._instantiation import InstanceTree

    if not found and not current_extends and isinstance(scope, (ast.InstanceClass, InstanceTree)):
        # Not found in instance tree, look in class tree
        found = _find_name(
            name=name,
            scope=scope.ast_ref,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            search_imports=search_imports,
            search_parent=search_parent,
            search_inherited=search_inherited,
            check_encapsulated=check_encapsulated,
        )

    return found


def _parse_str_or_ref(name: Union[str, ast.ComponentRef]) -> Tuple[str, str]:
    """Return (left_name, rest_of_name) given composite name as a str or ComponentRef"""
    assert isinstance(name, (str, ast.ComponentRef))
    if isinstance(name, str):
        left_name, _, rest_of_name = name.partition(".")
    else:
        name_parts = name.to_tuple()
        left_name = name_parts[0]
        rest_of_name = ".".join(name_parts[1:])
    return left_name, rest_of_name


def _instantiate_class_if_needed_for_lookup(
    class_: Union[ast.Class, ast.InstanceClass],
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
) -> Union[ast.Class, ast.InstanceClass]:
    """Instantiate class as needed for name lookup"""
    # If not an instance, then all names are already available for lookup
    if isinstance(class_, ast.Tree) or not isinstance(class_, ast.InstanceClass):
        return class_
    # Same if already at least partially instantiated
    if class_.fully_instantiated or class_.partially_instantiated:
        return class_
    # Only instantiate if we are not already instantiating this class
    # If a name is not available yet in class_ in process of being instantiated,
    # then name lookup will fall back to looking for it in the AST
    if current_instances is not None and class_ in current_instances:
        return class_
    # Late import to avoid circular dependency
    from ._instantiation import _instantiate_class

    instance = _instantiate_class(
        class_,
        ast.ClassModification(),
        class_.parent_instance,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
        partially=True,
    )
    return instance
    # TODO: Full instantiation in place for flattening
    # return _instantiate_class(
    #     class_,
    #     ast.ClassModification(),
    #     class_.parent_instance,
    # )


def _find_simple_name(
    name: str,
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Lookup name per Modelica spec 3.5 section 5.3.1 Simple Name Lookup"""

    # 1. Iteration variables
    # 2. Classes
    # 3. Components (Symbols in Pymoca)
    # 4. Classes and Components from Extends Clauses
    # 5. Qualified Import names, see 4 (but not from Extends Clauses) (spec 13.2.1)
    # 6. Public Unqualified Imports (error if multiple are found) (spec 13.2.1)
    # 7. a) Repeat 1-6 for each lexically enclosing instance scope,
    #    b) stopping at `encapsulated`
    #    c) unless predefined type, function, operator then look in root.
    #    d) If name matches a variable (a.k.a. component, symbol) in an enclosing class, it
    #       must be a `constant`.

    # Steps 1 - 7
    current_scope = scope

    # Search through enclosing scopes until we find something or hit a boundary
    while True:

        # FIXME: Remove if not needed
        if instantiate_in_place:
            current_scope = _instantiate_class_if_needed_for_lookup(
                current_scope,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )

        # Steps 1-3: Try local lookup first (iteration vars, classes, symbols)
        if found := _find_local(name, current_scope):
            break

        # Step 4: Look in inherited classes if enabled
        if search_inherited:
            if found := _find_inherited(
                name,
                current_scope,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            ):
                break

        # Steps 5-6: Look in imports if enabled
        if search_imports:
            if found := _find_imported(
                name,
                current_scope,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            ):
                break

        # Step 7b: Continue unless we should stop
        if not search_parent or not current_scope.parent or current_scope.encapsulated:
            break

        # Step 7a: Move up to parent scope and continue searching
        current_scope = current_scope.parent

    # Step 7c: If not found and we stopped at an encapsulated class,
    # then search predefined types, functions, and operators in global scope
    # TODO: Add predefined functions and operators to global scope before this
    if found is None and current_scope.encapsulated:
        found = _find_local(name, scope.root)
        if not isinstance(found, ast.Class) or found.type not in ("type", "function", "operator"):
            found = None

    # Step 7d: If name matches a variable (a.k.a. component, symbol) in an enclosing class,
    # it must be a `constant`.
    if (
        isinstance(found, ast.Symbol)
        and current_scope != scope
        and "constant" not in found.prefixes
    ):
        raise NameLookupError("Non-constant Symbol found in enclosing class")

    return found


def _find_rest_of_name(
    first: Union[ast.Class, ast.Symbol],
    rest_of_name: str,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Lookup the `B.C` part of Composite Name Lookup (`A.B.C`) (spec 5.3.2)"""

    # 1. `A` is looked up using Simple Name Lookup and passed as `first` argument
    # 2. If `A` is a Component:
    #     1. `B.C` is looked up from named component elements of `A`
    #     2. if not found and if `A.B.C` is used as a function call and `A` is a scalar or can be
    #     evaluated as a scalar from an array and `B` and `C` are classes,
    #     it is a non-operator function call.
    # 3. If `A` is a Class:
    #     1. `A` is temporarily flattened without modifiers of this class
    #     2. `B.C` is looked up among named elements of temp flattened class,
    #     but if `A` is not a package, lookup is restricted to `encapsulated` elements only
    #     and "the class we look inside shall not be partial in a simulation model".
    if isinstance(first, ast.Symbol):
        # Find the symbol type
        if isinstance(first.type, ast.Class):
            type_class = first.type
        else:
            type_class = _find_name(
                first.type,
                first.parent,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
            if type_class is None:
                raise NameLookupError(f"Lookup failed for type of symbol {first.full_name}")
        found = _find_composite_name_in_symbols(
            rest_of_name,
            type_class,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            search_imports=search_imports,
            search_parent=search_parent,
            search_inherited=search_inherited,
            check_encapsulated=check_encapsulated,
        )
        if not found:
            # Can only find in classes if the below rules apply:
            # 2b. if not found and if `A.B.C` is used as a function call
            # and `A` is a scalar or can be evaluated as a scalar from an array
            # and `B` and `C` are classes,
            # it is a non-operator function call.
            found, _ = _find_composite_name_in_classes(
                rest_of_name,
                type_class,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )
            if isinstance(found, ast.Class):
                if found.type != "function":
                    found = None
                else:
                    # TODO: Fix for `test_function_lookup_via_array_element` + other possibilities
                    if first.dimensions[0][0].value is not None:
                        raise NameLookupError(
                            f"Array {first.name} must have subscripts to lookup function {found.name}"
                        )

    elif isinstance(first, ast.Class):
        if not instantiate_in_place:
            found = _flatten_first_and_find_rest(
                first,
                rest_of_name,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=True,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
        else:
            found = _find_name(
                rest_of_name,
                first,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
        # Check that found meets non-package lookup requirements in spec section 5.3.2
        # The found.name test is so we only check going left to right in composite name
        # and not the other direction as we pop the recursive call stack.
        if (
            check_encapsulated
            and found is not None
            and found.name == _first_name(rest_of_name)
            and first.type != "package"
            and not (isinstance(found, ast.Class) and found.encapsulated)
        ):
            raise NameLookupError(
                f"{first.name} is not a package so {found.name} must be encapsulated"
            )

    else:
        raise NameLookupError(f'Found unexpected node "{first!r}" during name lookup')

    return found


def _find_composite_name_in_symbols(
    name: str,
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[ast.Symbol]:
    """Search for composite name (e.g. A.B.C) in local symbols, recursively"""
    if instantiate_in_place:
        scope = _instantiate_class_if_needed_for_lookup(
            scope,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )

    first_name, _, next_names = name.partition(".")

    # See spec 5.3.2 bullet 2 (emphasis mine): "If the first identifier denotes
    # a component, the rest of the name (e.g., B or B.C) is looked up among the
    # declared named *component* elements of the component".
    # This can include inherited and imported components.
    # Look up the type (Class) within the current scope if necessary
    found = _find_name(
        first_name,
        scope,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=instantiate_in_place,
        search_imports=search_imports,
        search_parent=False,
        search_inherited=search_inherited,
        check_encapsulated=check_encapsulated,
    )
    if isinstance(found, ast.Symbol):
        if next_names:
            if isinstance(found.type, ast.ComponentRef):
                type_name = str(found.type)
                found_type_class = _find_name(
                    type_name,
                    scope,
                    current_instances=current_instances,
                    current_extends=current_extends,
                    instantiate_in_place=instantiate_in_place,
                    search_imports=search_imports,
                    search_parent=search_parent,
                    search_inherited=search_inherited,
                    check_encapsulated=check_encapsulated,
                )
                if found_type_class is None or isinstance(found_type_class, ast.Symbol):
                    raise NameLookupError(
                        f'Symbol type "{type_name}" not found in scope "{scope.full_name}"'
                    )
            else:
                # type is already an InstanceClass
                found_type_class = found.type
            # Look in symbols of the type
            found = _find_composite_name_in_symbols(
                next_names,
                found_type_class,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
    else:
        found = None
    return found


def _find_composite_name_in_classes(
    name: str,
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    return_last_found=False,
) -> Tuple[Optional[ast.Class], str]:
    """Search for composite name (e.g. A.B.C) in local classes, recursively"""
    if instantiate_in_place:
        scope = _instantiate_class_if_needed_for_lookup(
            scope,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )

    first_name, _, next_names = name.partition(".")
    found = None
    if first_name in scope.classes:
        found = scope.classes[first_name]
    else:
        next_names = name
    if found and next_names:
        next_found, next_names = _find_composite_name_in_classes(
            next_names,
            found,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
        )
        if next_found:
            found = next_found
        elif not return_last_found:
            found = None
    return found, next_names


def _find_composite_name(
    name: str,
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Symbol, ast.Class]]:
    """Composite name lookup using partial instantiation only"""
    # Recurse through children classes
    found, next_names = _find_composite_name_in_classes(
        name,
        scope,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=True,
        return_last_found=True,
    )
    # Recurse through children symbols, if any
    if found and next_names:
        found = _find_composite_name_in_symbols(
            next_names,
            found,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=True,
            search_imports=search_imports,
            search_parent=search_parent,
            search_inherited=search_inherited,
            check_encapsulated=check_encapsulated,
        )

    return found


def _flatten_first_and_find_rest(
    first: ast.Class,
    rest_of_name: str,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Lookup the `B.C` part of Composite Name Lookup (`A.B.C`) where`A` is a Class"""

    # 3. If `A` is a Class:
    #     1. `A` is temporarily flattened without modifiers of this class
    #     2. `B.C` is looked up among named elements of temp flattened class,
    #     but if `A` is not a package, lookup is restricted to `encapsulated` elements only
    #     and "the class we look inside shall not be partial in a simulation model".

    #     Checking "the class we look inside shall not be partial in a simulation model"
    #     is left to the caller.

    # Late imports to avoid circular dependency
    from ._instantiation import _get_lexical_parent_instance, _instantiate_class
    from ._flattening import _create_partial_flat_instance, _flatten_instance

    # Per spec v3.5 section 5.3.2 bullet 4, class is temporarily flattened
    if not isinstance(first, ast.InstanceClass) or not first.partially_instantiated:
        # FIXME: Remove commented out code
        # if not instantiate_in_place:
        parent_instance = getattr(first, "parent_instance", None)
        if parent_instance is None:
            parent_instance = _get_lexical_parent_instance(
                first,
                first.parent,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
            )
        first_instance = _instantiate_class(
            first,
            ast.ClassModification(),
            parent_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            partially=True,
        )
    else:
        first_instance = first
    flat_class = _create_partial_flat_instance(first_instance)
    _flatten_instance(
        first_instance,
        flat_class,
        current_instances=current_instances,
        current_extends=current_extends,
        instantiate_in_place=True,
    )
    # Need non-flat name for name lookup
    flat_class.name = flat_class.name.split(".")[-1]
    first = flat_class

    if rest_of_name in flat_class.symbols:
        # Flattening allows us to bypass standard name lookup if all symbols
        found = flat_class.symbols[rest_of_name]
        # Restore non-flat name of instance (last name if compound name)
        found.name = found.name.split(".")[-1]
    else:
        # Classes with embedded references to the same class cause infinite recursion if we do
        # instantiation or temporary flattening recursively, so it is only done on the first
        # in a composite name.
        # The spec is a ambiguous about what to do when looking up the rest.
        # FIXME: Remove commented out code
        found = _find_name(
            rest_of_name,
            first_instance,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            search_imports=search_imports,
            search_parent=False,
            search_inherited=search_inherited,
            check_encapsulated=check_encapsulated,
        )
        # We will use the instantiate_in_place option look up the rest.
        # found = _find_composite_name(
        #     rest_of_name,
        #     first,
        #     current_instances=current_instances,
        #     current_extends=current_extends,
        #     instantiate_in_place=True,
        #     search_imports=search_imports,
        #     search_parent=False,
        #     search_inherited=search_inherited,
        #     check_encapsulated=check_encapsulated,
        # )

    return found


def _first_name(name: str) -> str:
    return name.split(".")[0]


def _find_local(
    name: str,
    scope: ast.Class,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Name lookup for predefined classes and contained elements"""

    # 1. Iteration variables
    # 2. Classes
    # 3. Components (Symbols in Pymoca)

    # 1. Iteration variables
    # TODO: Refactor when handling iteration variables (it will move up one level)
    if found := _find_iteration_variable(name, scope):
        return found

    # 2. Classes
    if name in scope.classes:
        return scope.classes[name]

    # 3. Components (Symbols in Pymoca)
    if name in scope.symbols:
        return scope.symbols[name]

    return None


def _find_iteration_variable(name: str, scope: ast.Class) -> Optional[ast.Symbol]:
    """Currently a pass"""
    # TODO: Implement find name in iteration variables
    return None


def _find_inherited(
    name: str,
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Find simple name in inherited classes"""
    for extends in scope.extends:
        # Avoid infinite recursion by keeping track of where we have been with current_extends
        # A common case is when multiple classes in the same hierarchy extend the same class
        # such as Icons in the Modelica Standard Library
        if current_extends:
            if extends in current_extends:
                continue
        else:
            current_extends = set()
        current_extends.add(extends)

        if isinstance(extends, ast.InstanceClass):
            return _find_name(
                name,
                extends,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )

        extends_scope = _find_name(
            extends.component,
            scope,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            search_imports=search_imports,
            search_parent=search_parent,
            search_inherited=search_inherited,
            check_encapsulated=check_encapsulated,
        )
        if extends_scope is not None:
            if isinstance(extends_scope, ast.Symbol):
                continue
            found = _find_name(
                name,
                extends_scope,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=False,
                search_parent=False,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
            current_extends.remove(extends)
            if found is not None:
                return found
        else:
            current_extends.remove(extends)
    return None


def _find_imported(
    name: str,
    scope: ast.Class,
    current_instances: Optional[Set[ast.InstanceClass]],
    current_extends: Optional[Set[Union[ast.ExtendsClause, ast.InstanceClass]]],
    instantiate_in_place: bool,
    search_imports: bool = True,
    search_parent: bool = True,
    search_inherited: bool = True,
    check_encapsulated: bool = True,
) -> Optional[Union[ast.Class, ast.Symbol]]:
    """Find simple name in imports per MLS v3.5 section 13.2.1"""
    # TODO: Rewrite this to work with parser rewrite of import_clause handler.
    # TODO: Can we do a scope.imports[name] = found Class or Symbol to speed up future calls?
    # Search qualified imports (most common case)
    if name in scope.imports:
        import_: Union[ast.ImportClause, ast.ComponentRef] = scope.imports[name]
        if isinstance(import_, ast.ImportClause):
            # TODO: Handle import of multiple classes (now only does `A.B.C` for `A.B.{C,D,E}`)
            import_ = import_.components[0]
        if scope.full_name == str(import_):
            raise NameLookupError(f"Import {import_} in scope {scope.full_name} is recursive")
        # FIXME: Make this and copy below DRY
        # Detect self-referential imports to prevent infinite recursion during instantiation
        common_parent, children = _get_common_parent(scope, str(import_))
        if common_parent is not None:
            found = _find_composite_name(
                children,
                common_parent,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=search_imports,
                search_parent=search_parent,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
            _check_import_rules(found, scope)
            return found

        found = _find_name(
            import_,
            scope.root,
            current_instances=current_instances,
            current_extends=current_extends,
            instantiate_in_place=instantiate_in_place,
            search_imports=search_imports,
            search_parent=False,
            search_inherited=search_inherited,
            check_encapsulated=check_encapsulated,
        )
        _check_import_rules(found, scope)
        return found
    # Unqualified imports
    if "*" in scope.imports:
        c = None
        for package_ref in scope.imports["*"].components:
            imported_comp_ref = package_ref.concatenate(ast.ComponentRef(name=name))
            # Search within the package
            # Detect self-referential imports to prevent infinite recursion during instantiation
            common_parent, children = _get_common_parent(scope, str(imported_comp_ref))
            if common_parent is not None:
                found = _find_composite_name(
                    children,
                    common_parent,
                    current_instances=current_instances,
                    current_extends=current_extends,
                    instantiate_in_place=instantiate_in_place,
                    search_imports=search_imports,
                    search_parent=search_parent,
                    search_inherited=search_inherited,
                    check_encapsulated=check_encapsulated,
                )
                _check_import_rules(found, scope)
                return found
            # Avoid infinite recursion with search_imports = False
            c = _find_name(
                imported_comp_ref,
                scope.root,
                current_instances=current_instances,
                current_extends=current_extends,
                instantiate_in_place=instantiate_in_place,
                search_imports=False,
                search_parent=False,
                search_inherited=search_inherited,
                check_encapsulated=check_encapsulated,
            )
            # TODO: Should _check_import_rules be inside `if c is not None` check? (fix in rewrite)
            _check_import_rules(c, scope)
            if c is not None:
                # Store result for next lookup
                scope.imports[name] = imported_comp_ref
                return c
    return None


def _get_common_parent(class_: ast.Class, name: str) -> Tuple[Optional[ast.Class], str]:
    common_prefix = os.path.commonprefix([class_.full_name, str(name)]).rstrip(".")
    if common_prefix:
        last_name = common_prefix.split(".")[-1]
        start_index = name.find(last_name)
        names = name[start_index:].split(".")
        parent_name = names[0]
        # Find parent_name in parents
        parent = class_
        while parent.name != parent_name:
            parent = parent.parent
        child_names = names[1:]
        return parent, ".".join(child_names)
    return (None, "")


def _check_import_rules(
    element: Optional[Union[ast.Class, ast.Symbol]],
    scope: ast.Class,
) -> None:
    """Check import rules per the Modelica spec"""
    if element is None:
        return
    # TODO: Is `not element.parent` a sufficient check for the error message? (fix in rewrite)
    if not element.parent:
        raise NameLookupError(
            f"Import {element.name} in scope {scope.full_name} must be contained in a package"
        )
    if element.parent.type != "package":
        if element.parent.name:
            message = (
                f"{element.parent.name} must be a package in "
                f"import {element.full_name} in scope {scope.full_name}"
            )
        else:
            message = (
                f"{element.full_name} is not in a package "
                f"so can't be imported in scope {scope.full_name}"
            )
        raise NameLookupError(message)
    if element.visibility != ast.Visibility.PUBLIC:
        raise NameLookupError(
            f"Import {element.name} must not be protected in scope {scope.full_name}"
        )
    if scope.full_name == element.full_name:
        raise NameLookupError(f"Import {element.full_name} is recursive in scope {scope.full_name}")
