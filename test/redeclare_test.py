#!/usr/bin/env python
"""
Tests for redeclaration semantics.
"""

import os

from conftest_parse import (
    MODEL_DIR,
    check_redeclare_expects,
    parse_and_instantiate_model,
    parse_model_files,
    redeclare_expect,
)

from pymoca import parser
from pymoca import tree

import pytest


@pytest.mark.xfail  # `redeclare class extends` not implemented
def test_flattening_redeclare_class_extends():
    """Test redeclare class extends construct"""

    _ = parse_and_instantiate_model("RedeclareClassExtends.mo", "P.TestOK")

    # TODO: Test equations

    with pytest.raises(tree.ModelicaSemanticError, match="TODO: FILL IN ERROR MESSAGE"):
        _ = parse_and_instantiate_model("RedeclareClassExtends.mo", "P.TestFail")


def test_redeclare_class():
    """Test redeclaration of class of contained components"""

    instance = parse_and_instantiate_model("RedeclareClass.mo", "Package.Model")

    expect = [
        redeclare_expect("b0.b", "Boolean", False, False),
        redeclare_expect("b1.b", "Integer", 3, False),
        redeclare_expect("b2.b", "Boolean", False, False),
    ]
    check_redeclare_expects(instance, expect)

    # Symbols themselves were not declared replaceable
    for name in ("b0", "b1", "b2"):
        assert not instance.symbols[name].replaceable


def test_redeclare_components():
    """Test redeclaration of components of same type"""

    instance = parse_and_instantiate_model("RedeclareComponents.mo", "Package.Model")

    expect = [
        redeclare_expect("b1.x", "Integer", 1, False),
        redeclare_expect("b2.x", "Integer", 2, False),
        redeclare_expect("b0.x", "Real", 0.0, True),
    ]
    check_redeclare_expects(instance, expect)

    # Symbols themselves were not declared replaceable
    for name in ("b1", "b2", "b0"):
        assert not instance.symbols[name].replaceable


def test_redeclare_component_in_extends():
    """Test redeclaration of components in extends clause"""

    instance = parse_and_instantiate_model("RedeclareComponentInExtends.mo", "M")

    expect = [
        redeclare_expect("M.d1.x", "Integer", 1, False),
        redeclare_expect("M.d2.x", "Integer", 2, False),
        redeclare_expect("M.d3.x", "Integer", 3.0, True),
        redeclare_expect("M.d4.x", "Integer", 4.0, True),
        redeclare_expect("M.d5.x", "String", "5", True),
    ]
    check_redeclare_expects(instance, expect)

    # Symbols themselves were not declared replaceable
    for name in ("d1", "d2", "d3", "d4", "d5"):
        assert not instance.symbols[name].replaceable


def test_redeclare_component_in_declaration():
    """Test redeclaration of components in component declaration"""
    instance = parse_and_instantiate_model("RedeclareComponentInDeclaration.mo", "M")

    expect = [
        redeclare_expect("M.ir1.x", "Real", 1.0, False),
        redeclare_expect("M.ir2.x", "Real", 2.0, False),
        redeclare_expect("M.ir3.x", "Real", 3.0, True),
        redeclare_expect("M.ir4.x", "Real", 4.0, True),
        redeclare_expect("M.ir5.x", "Real", 5.0, True),
    ]
    check_redeclare_expects(instance, expect)

    # Symbols themselves were not declared replaceable
    for name in ("ir1", "ir2", "ir3", "ir4", "ir5"):
        assert not instance.symbols[name].replaceable, f"{name} is replaceable"


def test_redeclare_component_complicated():
    """Test more complicated cases of redeclaring components

    This includes direct and indirect redeclarations, multiple levels of inheritance
    with modifications, and redeclarations of components with different types.
    """

    instance = parse_and_instantiate_model("RedeclareComponentComplicated.mo", "P.M")
    expect = [
        redeclare_expect("num", "Integer", 4, True),
        redeclare_expect("b.num", "Integer", 5, True),
        redeclare_expect("f.num", "Real", 6.0, False),
        redeclare_expect("f2.num", "Integer", 1, True),
    ]
    check_redeclare_expects(instance, expect)

    # Symbols themselves were declared replaceable
    for name in ("f", "f2"):
        assert instance.symbols[name].replaceable, f"{name} not replaceable"

    instance = parse_and_instantiate_model("RedeclareComponentComplicated.mo", "P.M2")
    expect = [
        redeclare_expect("num", "Real", 7.0, False),
        redeclare_expect("b.num", "Integer", 5, True),
        # G is `type G = Real`, so resolves to Real at the symbol level
        redeclare_expect("g", "Real", 8.0, False),
    ]
    check_redeclare_expects(instance, expect)


def test_redeclare_component_type_compatibility():
    """Test type compatibility for redeclaration of components of builtin types"""

    # TODO: Update when new flattening is implemented
    # For now we expect the values from the base class modifications.
    # Flattening should catch the value assignment errors noted in the model
    # when the modifiers are flattened.

    instance = parse_and_instantiate_model("RedeclareTypeCompatibility.mo", "M")
    expect = [
        redeclare_expect("b2i.x", "Integer", True, True),
        redeclare_expect("b2r.x", "Real", True, True),
        redeclare_expect("b2s.x", "String", True, True),
        redeclare_expect("i2r.x", "Real", -1, True),
        redeclare_expect("i2s.x", "String", -1, True),
        redeclare_expect("r2i.x", "Integer", 3.5, True),
        redeclare_expect("r2b.x", "Boolean", 3.5, True),
        redeclare_expect("r2s.x", "String", 3.5, True),
        redeclare_expect("sr2b.x", "Boolean", 3.5, True),
        redeclare_expect("sr2i.x", "Integer", 3.5, True),
        redeclare_expect("ss2r.x", "Real", "foo", True),
        redeclare_expect("ss2rv.x", "Real", 3.5, True),
    ]
    check_redeclare_expects(instance, expect)


def test_redeclare_class_with_symbol_error():
    """Test redeclaration of a class with a symbol is disallowed"""

    txt = """
        model A
            model B
                replaceable model C
                    Real c;
                end C;
                C b;
            end B;
            B a;
            B b(redeclare model C = a); // Error: redeclare class C with component a
        end A;
    """
    ast_tree = parser.parse(txt)
    with pytest.raises(
        tree.ModelicaSemanticError, match="Redeclaring C with component a in scope A"
    ):
        _ = tree.instantiate("A", ast_tree)


def test_missing_redeclare_class_error():
    """Test redeclaration with a class that can't be found"""

    txt = """
        model A
            model B
                replaceable model C
                    Real c;
                end C;
                C b;
            end B;
            B b(redeclare model C = D); // Error: D doesn't exist
        end A;
    """
    ast_tree = parser.parse(txt)
    with pytest.raises(tree.NameLookupError, match="Redeclare class D not found in scope A"):
        _ = tree.instantiate("A", ast_tree)


def test_redeclare_in_extends():
    instance = parse_and_instantiate_model("RedeclareInExtends.mo", "ChannelZ")

    assert "Z" in instance.extends[0].symbols["down"].type.symbols

    flat_tree = tree.flatten_instance(instance)

    assert "down.Z" in flat_tree.symbols


def test_redeclaration_scope():
    instance = parse_and_instantiate_model("RedeclarationScope.mo", "ChannelZ")

    c_type = instance.symbols["c"].type
    assert "Z" in c_type.symbols["up"].type.extends[0].symbols
    assert "A" in c_type.symbols["down"].type.symbols

    flat_tree = tree.flatten_instance(instance)

    assert "c.up.Z" in flat_tree.symbols
    assert "c.down.A" in flat_tree.symbols


def test_redeclaration_scope_alternative():
    instance = parse_and_instantiate_model("RedeclarationScopeAlternative.mo", "ChannelZ")

    c_type = instance.symbols["c"].type
    assert "Z" in c_type.symbols["up"].type.extends[0].symbols
    assert "A" in c_type.symbols["down"].type.symbols

    flat_tree = tree.flatten_instance(instance)

    assert "c.up.Z" in flat_tree.symbols
    assert "c.down.A" in flat_tree.symbols


def test_extends_redeclareable():
    ast_tree = parser.parse_file(os.path.join(MODEL_DIR, "ExtendsRedeclareable.mo"))

    with pytest.raises(
        tree.ModelicaSemanticError, match="In D extends C, C and parents cannot be replaceable"
    ):
        instance = tree.instantiate("E", ast_tree)  # noqa: F841


def test_redeclare_nonreplaceable():
    ast_tree = parser.parse_file(os.path.join(MODEL_DIR, "RedeclareNonReplaceable.mo"))

    with pytest.raises(tree.ModelicaSemanticError, match="Redeclaring D.C that is not replaceable"):
        instance = tree.instantiate("E", ast_tree)  # noqa: F841


def test_redeclare_nested():
    with pytest.raises(parser.ModelicaSyntaxError, match="mismatched input '.' expecting '='"):
        _ = parse_model_files("RedeclareNestedClass.mo.fail_parse")


if __name__ == "__main__":
    import pytest as _pytest

    _pytest.main([__file__])
