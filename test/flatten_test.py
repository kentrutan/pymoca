#!/usr/bin/env python
"""
Tests for tree.flatten_instance() / tree.flatten().
"""

from conftest_parse import (
    _flush,
    parse_and_instantiate_model,
    parse_model_files,
)

from pymoca import ast
from pymoca import parser
from pymoca import tree

import pytest


def test_aircraft():
    ast_tree = parse_model_files("Aircraft.mo")
    print("AST TREE\n", ast_tree)
    instance = tree.instantiate("Aircraft", ast_tree)
    flat_tree = tree.flatten_instance(instance)
    print("AST TREE FLAT\n", flat_tree)
    _flush()


def test_bouncing_ball():
    ast_tree = parse_model_files("BouncingBall.mo")
    print("AST TREE\n", ast_tree)
    instance = tree.instantiate("BouncingBall", ast_tree)
    flat_tree = tree.flatten_instance(instance)
    print(flat_tree)
    print("AST TREE FLAT\n", flat_tree)
    _flush()


def test_estimator():
    ast_tree = parse_model_files("./Estimator.mo")
    print("AST TREE\n", ast_tree)
    instance = tree.instantiate("Estimator", ast_tree)
    flat_tree = tree.flatten_instance(instance)
    print("AST TREE FLAT\n", flat_tree)
    _flush()


def test_spring():
    ast_tree = parse_model_files("Spring.mo")
    print("AST TREE\n", ast_tree)
    instance = tree.instantiate("Spring", ast_tree)
    flat_tree = tree.flatten_instance(instance)
    print("AST TREE FLAT\n", flat_tree)
    _flush()


def test_duplicate_state():
    ast_tree = parse_model_files("DuplicateState.mo")
    print("AST TREE\n", ast_tree)
    instance = tree.instantiate("DuplicateState", ast_tree)
    flat_tree = tree.flatten_instance(instance)
    print("AST TREE FLAT\n", flat_tree)
    _flush()


def test_function_pull():
    ast_tree = parse_model_files("FunctionPull.mo")

    class_name = "Level1.Level2.Level3.Function5"
    comp_ref = ast.ComponentRef.from_string(class_name)

    flat_tree = tree.flatten(ast_tree, comp_ref)

    # Check if all referenced functions are pulled in
    assert "Level1.Level2.Level3.f" in flat_tree.classes
    assert "Level1.Level2.Level3.TestPackage.times2" in flat_tree.classes
    assert "Level1.Level2.Level3.TestPackage.square" in flat_tree.classes
    assert "Level1.Level2.Level3.TestPackage.not_called" not in flat_tree.classes

    # Check if the classes in the flattened tree have the right type
    assert flat_tree.classes["Level1.Level2.Level3.Function5"].type == "model"

    assert flat_tree.classes["Level1.Level2.Level3.f"].type == "function"
    assert flat_tree.classes["Level1.Level2.Level3.TestPackage.times2"].type == "function"
    assert flat_tree.classes["Level1.Level2.Level3.TestPackage.square"].type == "function"

    # Check whether input/output information of functions comes along properly
    func_t2 = flat_tree.classes["Level1.Level2.Level3.TestPackage.times2"]
    assert "input" in func_t2.symbols["x"].prefixes
    assert "output" in func_t2.symbols["y"].prefixes

    # Check if built-in function call statement comes along properly
    func_f = flat_tree.classes["Level1.Level2.Level3.f"]
    assert func_f.statements[0].right.operator == "*"
    # Check if user-specified function call statement comes along properly
    assert (
        func_f.statements[0].right.operands[0].operator == "Level1.Level2.Level3.TestPackage.times2"
    )


def test_flattening_inheritance_tree():
    """Test flattening multi-level class hierarchy with modifications but no equations"""
    instance = parse_and_instantiate_model("InstantiationTree.mo", "TreeModel.Tree")
    flat_tree = tree.flatten_instance(instance)
    expect = (
        ("w", "nominal", 2.0),
        ("b.t", "nominal", 1.0),
        ("l.c", "value", 1),
        ("l2.c", "value", 2),
        ("t", "nominal", 2.0),
    )
    for name, attr, value in expect:
        assert name in flat_tree.symbols
        symbol = flat_tree.symbols[name]
        assert getattr(symbol, attr) == value


def test_flattening_spring_system():
    """Test flattening of a simple class hierarchy with equations"""
    instance = parse_and_instantiate_model("SpringSystemExample.mo", "Example.SpringSystem")
    flat_tree = tree.flatten_instance(instance)
    for name in ("spring.x", "spring.f", "damper.v", "damper.f", "damper.c"):
        assert name in flat_tree.symbols, f"Name not flattened: {name}"


def test_flattening_modification_scope():
    """Test for correct scope for references and values of modifications"""
    instance = parse_and_instantiate_model("ModificationScope.mo", "A")
    flat_tree = tree.flatten_instance(instance)
    # Check that the symbol value set by modifications have the right value and scope
    expect = (  # symbol_name, value_type, value, value_parent_name
        ("R", "literal", 3.0, None),  # Literal has no value parent
        ("a.R", "literal", 4.0, "A"),
        ("b.R", "symbol", "R", ""),  # Unnamed extends node is parent
        ("c.R", "literal", 5.0, "A"),
        ("d.R", "symbol", "R", ""),  # Unnamed extends node is parent
    )
    for symbol_name, value_type, value, value_parent_name in expect:
        symbol_value = flat_tree.symbols[symbol_name].value
        if value_type == "literal":
            assert symbol_value == value, "Wrong value for symbol: " + symbol_name
        elif value_type == "symbol":
            # Check that we have the correct reference for symbolic values
            assert isinstance(symbol_value, ast.Symbol)
            assert symbol_value.name == value, "Wrong value for symbol: " + symbol_name
            # Check that we have the correct scope for symbolic values
            assert symbol_value.parent_instance.name == value_parent_name, (
                "Wrong value scope for symbol: " + symbol_name
            )
        else:
            raise AssertionError(f"Unexpected value type in test: {value_type}")


def test_extends_order():
    instance = parse_and_instantiate_model("ExtendsOrder.mo", "P.M")

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["at.m"].value == 0.0


def test_constant_references():
    instance = parse_and_instantiate_model("ConstantReferences.mo", "b")

    # Since b extends a and a has no symbols, new instantiation stops there
    # Instead we will check for the redeclare being applied correctly
    b_m_mod = instance.extends[0].classes["m"].modification_environment.arguments[0]
    assert b_m_mod.value.name == "m"
    assert b_m_mod.value.component.name == "m2"

    flat_tree = tree.flatten_instance(instance)

    # TODO: Update after constant reference evaluation is implemented
    # This was the Pymoca v0.10 test:
    # self.assertEqual(flat_tree.symbols["m.p"].value, 2.0)
    # self.assertEqual(flat_tree.symbols["M2.m.f"].value, 3.0)

    # This is what we expect after the new flattening
    assert flat_tree.symbols["y"].value.name == "m.p"
    assert flat_tree.symbols["y"].value.parent_instance.name == ""  # Unnamed extends node
    assert flat_tree.symbols["z"].value.name == "P0.p"
    # TODO: Uncomment after equation references and constant references are implemented
    # self.assertIn("M2.m.f", flat_tree.symbols)
    # self.assertEqual(flat_tree.symbols["M2.m.f"].value.name, "m.f")
    # self.assertEqual(flat_tree.symbols["M2.m.f"].value.parent.name, "M1")
    # self.assertEqual(flat_tree.symbols["M2.m.f"].value.value, 3.0)


def test_parameter_modification_scope():
    instance = parse_and_instantiate_model("ParameterScope.mo", "ScopeTest")

    p_sym = instance.symbols["p"].type.symbols["Real"]
    p_sym_mod = p_sym.modification_environment.arguments[0]
    assert p_sym_mod.value.component.name == "value"
    assert p_sym_mod.value.modifications[0].value == 1.0
    nc_p_sym = instance.symbols["nc"].type.symbols["p"].type.symbols["Real"]
    nc_p_mod = nc_p_sym.modification_environment.arguments[0]
    assert nc_p_mod.value.modifications[0].name == "p"

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["nc.p"].value.name == "p"


def test_custom_units():
    instance = parse_and_instantiate_model("CustomUnits.mo", "A")

    dummy = instance.extends[0].symbols["dummy_parameter"].type.extends[0].symbols["Real"]
    dummy_value_mod = dummy.modification_environment.arguments[0]
    assert dummy_value_mod.value.component.name == "unit"
    assert dummy_value_mod.value.modifications[0].value == "m/s"
    dummy_value_mod = dummy.modification_environment.arguments[-1]
    assert dummy_value_mod.value.component.name == "value"
    assert dummy_value_mod.value.modifications[0].value == 10.0

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["dummy_parameter"].unit == "m/s"
    assert flat_tree.symbols["dummy_parameter"].value == 10.0


def test_extend_from_self():
    txt = """
    model A
      extends A;
    end A;"""

    ast_tree = parser.parse(txt)

    with pytest.raises(tree.ModelicaSemanticError, match="Cannot extend class 'A' with itself"):
        instance = tree.instantiate("A", ast_tree)  # noqa: F841

    # TODO: Update when new flattening is implemented
    class_name = "A"
    comp_ref = ast.ComponentRef.from_string(class_name)

    with pytest.raises(Exception, match="Cannot extend class 'A' with itself"):
        flat_tree = tree.flatten(ast_tree, comp_ref)  # noqa: F841


if __name__ == "__main__":
    import pytest as _pytest

    _pytest.main([__file__])
