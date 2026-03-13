#!/usr/bin/env python
"""
Parse-only and miscellaneous AST tests.
"""

import io
import re
import threading
import time

from conftest_parse import (
    _flush,
    parse_model_files,
)

from pymoca import ast
from pymoca import parser
from pymoca import tree

import pytest


def test_ast_element_full_name():
    """Test fully-qualified name lookup to element and back to name"""
    ast_tree = parse_model_files("TreeLookup.mo")
    full_names = (
        "Level1.Level2.Level3.TestPackage.TestClass",  # Class
        "Level1.Level2.Level3.TestPackage.c",  # Symbol
    )
    for name in full_names:
        element = tree.find_name(name, ast_tree)
        assert element is not None
        assert ast.element_full_name(element) == name


def test_connector():
    # ast_tree =
    parse_model_files("Connector.mo")
    # states = ast_tree.classes['Aircraft'].states
    # names = sorted([state.name for state in states])
    # names_set = sorted(list(set(names)))
    # if names != names_set:
    #     raise IOError('{:s} != {:s}'.format(str(names), str(names_set)))
    _flush()


def test_declarations_are_not_builtin_names():
    """Check that declaration names are not builtin types

    See MLS v3.5 section 4.8 Predefined Types and Classes
    """
    # Check that errors are caught in the parser
    error_templates = (
        # Class, model, function and type definitions
        "class {} end {};",
        "model {} end {};",
        "function {} end {};",
        "model A type {} = Real; end A;",
        # Component declarations
        "model A Real {}; end A;",
        "model A Real a, {}; end A;",
        "model A type B = {}; B {}; end A;",
        "model A model B end B; B {}; end A;",
        # Redeclares
        "model A B b(redeclare Real {}); model B Real x; end B; end A;",
        "model A B b(redeclare type {} = Integer); replaceable type B = {}; end A;",
    )
    for template in error_templates:
        for name in ast.Class.BUILTIN:
            txt = template.format(name, name)
            with pytest.raises(
                parser.ModelicaSyntaxError,
                match=f"Predefined type {name} not allowed as identifier",
            ):
                parser.parse(txt)

    ok_examples = (
        "model A parameter Integer i=1; String s=String(i); end A;",
        "model A type E = enumeration(a, b); parameter Integer i = Integer(E.a); end A;",
        'model A redeclare type Voltage = Real(unit="V"); end A;',
    )
    for txt in ok_examples:
        ast_tree = parser.parse(txt)
        assert ast_tree is not None, f"for '{txt}'"


def test_parse_nonexistent_file_error():
    """Test parse_file on non-existent file raises an error"""
    with pytest.raises(FileNotFoundError, match="File not found: NonExistentFile.mo"):
        parser.parse_file("NonExistentFile.mo")


def test_deep_copy_timeout():
    ast_tree = parse_model_files("DeepCopyTimeout.mo")

    # Start a background thread which will run the flattening, such that
    # we can kill it if takes to long.
    thread = threading.Thread(
        target=tree.flatten,
        args=(
            ast_tree,
            ast.ComponentRef(name="Test"),
        ),
    )

    # Daemon threads automatically stop when the program stops (and do not
    # prevent the program from exiting)
    thread.setDaemon(True)
    thread.start()

    # Use a timeout of 5 seconds. We check every 100 ms sec, such that the
    # test is fast to succeed when everything works as expected.
    for _ in range(50):
        time.sleep(0.1)
        if not thread.is_alive():
            return
    assert not thread.is_alive(), "Timeout occurred"


def test_visibility_in_ast():
    """Test visibility is set correctly in AST elements"""

    ast_tree = parse_model_files("Visibility.mo")
    A = ast_tree.classes["A"]

    # Test visibility of the classes
    assert ast.Visibility.PUBLIC == A.visibility
    assert ast.Visibility.PROTECTED == A.classes["B"].visibility
    assert ast.Visibility.PUBLIC == A.classes["C"].visibility

    # Test symbols visibility
    assert ast.Visibility.PROTECTED == A.symbols["b"].visibility
    assert ast.Visibility.PUBLIC == A.symbols["c"].visibility
    assert ast.Visibility.PUBLIC == A.symbols["d"].visibility
    assert ast.Visibility.PROTECTED == A.symbols["e"].visibility
    assert ast.Visibility.PROTECTED == A.symbols["f"].visibility

    # Test extends visibility
    C = A.classes["C"]
    assert ast.Visibility.PROTECTED == C.extends[0].visibility

    # Test visibility of public symbols in a protected class is as parsed
    B = A.classes["B"]
    for symbol in B.symbols.values():
        assert ast.Visibility.PUBLIC == symbol.visibility


def test_basemodelica_scalarized():
    """Test parsing BaseModelica example with scalar varables"""
    ast_tree = parse_model_files("BaseModelicaScalarized.mo")
    assert ast_tree is not None
    class_name = "'ManglingTest'"
    comp_ref = ast.ComponentRef.from_string(class_name)
    flat_tree = tree.flatten(ast_tree, comp_ref)
    # Check a few things, by no means exhaustive
    symbol = flat_tree.classes[class_name].symbols["'root.mm[1].p'"]
    assert symbol.value.value == 2.0
    assert "parameter" in symbol.prefixes
    found_y_equation = False
    for eqn in flat_tree.classes[class_name].equations:
        if isinstance(eqn.left, ast.Symbol) and eqn.left.name == "'y'":
            assert eqn.right.name == "'root.m.x'"
            found_y_equation = True
            break
    assert found_y_equation


def test_basemodelica_hierarchical():
    """Test parsing BaseModelica example with array variables"""
    ast_tree = parse_model_files("BaseModelicaHierarchical.mo")
    assert ast_tree is not None
    class_name = "'ManglingTest'"
    comp_ref = ast.ComponentRef.from_string(class_name)
    flat_tree = tree.flatten(ast_tree, comp_ref)
    pass
    # Check a few things, by no means exhaustive
    symbol = flat_tree.classes[class_name].symbols["'root'.'mm'.'p'"]
    dimensions = [dim.value for [dim] in symbol.dimensions]
    assert [None, 2, None] == dimensions
    values = [val.value for val in symbol.value.values]
    assert [2.0, 3.0] == values
    found_y_equation = False
    for eqn in flat_tree.classes[class_name].equations:
        if isinstance(eqn.left, ast.Symbol) and eqn.left.name == "'y'":
            assert eqn.right.name == "'root'.'m'.'x'"
            found_y_equation = True
            break
    assert found_y_equation


def test_signed_expression():
    """Test that both + and - prefix operators work in expressions"""
    txt = """
        model A
          parameter Integer iplus = +1;
          parameter Integer ineg = -iplus;
          parameter Real rplus = +1.0;
          parameter Real rneg = -1.0;
          parameter Real rboth = -1.0 - +1.0;
          parameter Boolean option = true;
          parameter Integer itest = if option then +2 else -2;
        end A;
    """

    # TODO: Update when new flattening is implemented
    ast_tree = parser.parse(txt)

    class_name = "A"
    comp_ref = ast.ComponentRef.from_string(class_name)

    flat_tree = tree.flatten(ast_tree, comp_ref)

    # Test that parses into correct expressions.
    symbols = flat_tree.classes["A"].symbols
    for sym in "iplus", "rplus":
        assert symbols[sym].value.operator == "+"
        assert len(symbols[sym].value.operands) == 1
    for sym in "ineg", "rneg":
        assert symbols[sym].value.operator == "-"
        assert len(symbols[sym].value.operands) == 1
    assert symbols["rboth"].value.operands[1].operator == "+"
    assert len(symbols["rboth"].value.operands[1].operands) == 1
    assert symbols["itest"].value.expressions[0].operator == "+"
    assert symbols["itest"].value.expressions[1].operator == "-"
    assert len(symbols["itest"].value.expressions[0].operands) == 1
    assert len(symbols["itest"].value.expressions[1].operands) == 1


def test_unit_type():
    txt = """
        model A
          parameter Integer x = 1;
          parameter Real y = 1.0;
          parameter Real z = 1;  // Mismatch
          parameter Integer w = 1.0;  // Mismatch
        end A;
    """

    ast_tree = parser.parse(txt)

    class_name = "A"
    comp_ref = ast.ComponentRef.from_string(class_name)

    flat_tree = tree.flatten(ast_tree, comp_ref)

    # For the moment, we do not raise errors/warnings or do
    # auto-conversions in the parser/flattener.
    assert isinstance(flat_tree.classes["A"].symbols["x"].value.value, int)
    assert isinstance(flat_tree.classes["A"].symbols["y"].value.value, float)
    # self.assertIsInstance(flat_tree.classes['A'].symbols['z'].value.value, int)
    # self.assertIsInstance(flat_tree.classes['A'].symbols['w'].value.value, float)


def test_unit_type_array():
    txt = """
        model A
          parameter Integer x[2, 2] = {{1, 2}, {3, 4}};
          parameter Real y[2, 2] = {{1.0, 2.0}, {3.0, 4.0}};
        end A;
    """

    ast_tree = parser.parse(txt)

    class_name = "A"
    comp_ref = ast.ComponentRef.from_string(class_name)

    flat_tree = tree.flatten(ast_tree, comp_ref)

    # For the moment, we leave type conversions to the backends. We only want to
    # be sure that we read in the correct type in the parser.
    for i in range(2):
        for j in range(2):
            assert isinstance(
                flat_tree.classes["A"].symbols["x"].value.values[i].values[j].value, int
            )
            assert isinstance(
                flat_tree.classes["A"].symbols["y"].value.values[i].values[j].value, float
            )


def test_class_comment():
    """Test that class comment/description is retained after flattening"""
    library_tree = parse_model_files("Aircraft.mo")
    comp_ref = ast.ComponentRef.from_string("Aircraft")
    flat_tree = tree.flatten(library_tree, comp_ref)
    aircraft = flat_tree.classes["Aircraft"]
    assert aircraft.comment == "the aircraft"
    assert aircraft.symbols["accel.a_x"].comment == "true acceleration"
    assert aircraft.symbols["accel.ma_x"].comment == "measured acceleration"
    assert aircraft.symbols["body.g"].comment == ""


def test_syntax_error():
    """Test ModelicaSyntaxError parse exception and related functions"""

    # Lexical syntax error at first character
    with pytest.raises(
        parser.ModelicaSyntaxError,
        match=r"token recognition error at: '`' \(input, line 1\)",
    ):
        _ = parser.parse("`", bypass_cache=True)

    # Syntax error at <EOF> without trailing newline
    with pytest.raises(
        parser.ModelicaSyntaxError,
        match=r"missing ';' at '<EOF>' \(input, line 2\)",
    ):
        _ = parser.parse("model A\nend A", bypass_cache=True)

    # Syntax error at <EOF> with trailing newline
    with pytest.raises(
        parser.ModelicaSyntaxError,
        match="missing ';' at '<EOF>' \\(input, line 3\\)",
    ):
        parser.parse("model A\nend A\n", bypass_cache=True)

    # Syntax error in a file
    with pytest.raises(
        parser.ModelicaSyntaxError,
        match=r"mismatched input '.' expecting '=' \(RedeclareNestedClass.mo.fail_parse, line 21\)",
    ) as exc_info:
        _ = parse_model_files("RedeclareNestedClass.mo.fail_parse")

    # Test print_syntax_error() of previous exception
    error_text = io.StringIO()
    parser.print_syntax_error(exc_info.value, error_text)
    expected_regex = (
        r".*RedeclareNestedClass\.mo\.fail_parse:21:48:",
        r"  extends D\(c.z.x\(nominal=2\), redeclare model C\.B=F\);",
        r"ModelicaSyntaxError: mismatched input '\.' expecting '='",
    )
    expected_regex = "\n".join(expected_regex)
    assert re.search(expected_regex, error_text.getvalue())


if __name__ == "__main__":
    import pytest as _pytest

    _pytest.main([__file__])
