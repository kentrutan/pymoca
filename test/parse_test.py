#!/usr/bin/env python
"""
Modelica parse Tree to AST tree.
"""

import contextlib
import enum
import io
import logging
import os
import pickle
import re
import sqlite3
import sys
import tempfile
import threading
import time
from collections import namedtuple
from pathlib import Path

import pymoca
from pymoca import ast
from pymoca import parser
from pymoca import tree
from pymoca.parser import DEFAULT_MODEL_CACHE_DB
from pymoca.tree._name_lookup import _find_name

import pytest

MY_DIR = os.path.dirname(os.path.realpath(__file__))
MODEL_DIR = os.path.join(MY_DIR, "models")
COMPLIANCE_DIR = os.path.join(MY_DIR, "libraries", "Modelica-Compliance", "ModelicaCompliance")
IMPORTS_DIR = os.path.join(COMPLIANCE_DIR, "Scoping", "NameLookup", "Imports")
MSL3_DIR = os.path.join(MY_DIR, "libraries", "MSL-3.2.3")
MSL4_DIR = os.path.join(MY_DIR, "libraries", "MSL-4.0.x")

redeclare_expect = namedtuple("redeclare_expect", ["name", "type", "value", "replaceable"])


class WorkDirState(enum.Enum):
    CLEAN = "clean"
    DIRTY = "dirty"


@contextlib.contextmanager
def modify_version(version_type: WorkDirState):
    pymoca_version = pymoca.__version__
    if pymoca_version.endswith(".dirty"):
        clean_version = pymoca_version[:-6]
    else:
        clean_version = pymoca_version

    dirty_version = clean_version + ".dirty"

    if version_type == WorkDirState.CLEAN:
        pymoca.__version__ = clean_version
    elif version_type == WorkDirState.DIRTY:
        pymoca.__version__ = dirty_version
    else:
        raise ValueError("Unknown version type")

    try:
        yield
    finally:
        pymoca.__version__ = pymoca_version


def check_instance_tree_is_all_instances(instance):
    """Check that all pertinent nodes in tree are instances.

    Return list of ones that are not."""

    class InstanceTreeListener(tree.TreeListener):
        def __init__(self):
            self.non_instances = []
            super().__init__()

        def exitClass(self, node):
            self.non_instances.append(node)

        def exitSymbol(self, node):
            self.non_instances.append(node)

        def exitExtendsClause(self, node):
            self.non_instances.append(node)

        def exitTree(self, node):
            self.non_instances.append(node)

    class InstanceTreeWalker(tree.TreeWalker):
        def skip_child(self, node: ast.Node, child_name: str) -> bool:
            if (
                isinstance(node, (ast.InstanceElement, tree.InstanceTree))
                and child_name == "ast_ref"
            ):
                return True
            return super().skip_child(node, child_name)

    listener = InstanceTreeListener()
    walker = InstanceTreeWalker()
    walker.walk(listener, instance.root)

    return listener.non_instances


def get_modifiers_by_name(symbol, name):
    """Get the modifiers given attribute name of given symbol instance"""
    assert isinstance(symbol, ast.InstanceSymbol), "Requires a symbol instance"
    arguments = []
    if symbol.type.name in ast.Tree.BUILTIN_TYPES:
        if isinstance(symbol.type, ast.ComponentRef):
            environment = symbol.modification_environment
        else:
            environment = symbol.type.symbols[symbol.type.name].modification_environment
    elif isinstance(symbol.type, ast.Class) and symbol.type.type == "type":
        type_sym = list(symbol.type.extends[0].symbols.values())[0]
        environment = type_sym.modification_environment
    else:
        environment = symbol.modification_environment
    for arg in environment.arguments:
        if arg.value.component.name == name:
            arguments.append(arg)
    return arguments


def _flush():
    sys.stdout.flush()
    sys.stdout.flush()
    time.sleep(0.1)


def parse_model_files(*pathnames):
    "Parse given files from MODEL_DIR and return parsed ast.Tree"
    ast_tree = None
    for path in pathnames:
        file_tree = parser.parse_file(os.path.join(MODEL_DIR, path))
        if ast_tree:
            ast_tree.extend(file_tree)
        else:
            ast_tree = file_tree
    return ast_tree


def parse_dir_files(directory, *pathnames):
    """Parse given file paths relative to dir and return parsed ast.Tree

    Dir is os-specific and paths are unix-style but are transformed to os specific.
    """
    ast_tree = None
    for pathname in pathnames:
        split_path = pathname.split("/")
        full_path = os.path.join(directory, *split_path)
        file_tree = parser.parse_file(full_path)
        if ast_tree:
            ast_tree.extend(file_tree)
        else:
            ast_tree = file_tree
    return ast_tree


def parse_and_instantiate(filename, class_name):
    ast_tree = parser.parse_file(filename)
    assert ast_tree is not None, f"Failed to parse {filename}"
    pickled_before = pickle.dumps(ast_tree)
    instance = tree.instantiate(class_name, ast_tree)
    assert instance is not None, f"Failed to instantiate {filename}"
    pickled_after = pickle.dumps(ast_tree)
    assert pickled_before == pickled_after, "AST was modified during instantiation"
    return instance


def parse_and_instantiate_model(filename, class_name):
    return parse_and_instantiate(os.path.join(MODEL_DIR, filename), class_name)


def parse_and_flatten_model(filename, class_name):
    instance = parse_and_instantiate(os.path.join(MODEL_DIR, filename), class_name)
    return tree.flatten_instance(instance)


def parse_imports_file(pathname):
    "Parse given path relative to IMPORTS_DIR and return parsed ast.Tree"
    arg_ast = parser.parse_file(os.path.join(IMPORTS_DIR, pathname))
    icon_ast = parser.parse_file(os.path.join(COMPLIANCE_DIR, "Icons.mo"))
    icon_ast.extend(arg_ast)
    return icon_ast


def check_redeclare_expects(instance, expects):
    """Check that the redeclare expectations are met in the given instance"""
    for name, type_, value, replaceable in expects:
        x = _find_name(name, instance, check_encapsulated=False)
        assert x is not None, f"{name} not found in instance"
        assert x.replaceable == replaceable, f"for {name}"
        if x.type.type == "type":
            x_type = x.type
        else:
            x_type = x.type.type
        # If redeclared, type is extends[0], possibly multiple levels down
        while x_type.type == "type":
            if x_type.extends:
                x_type = x_type.extends[0]
            else:
                break
        assert type_ in x_type.symbols, f"{name} not redeclared correctly"
        x_value_args = get_modifiers_by_name(x_type.symbols[type_], "value")
        assert len(x_value_args) > 0, f"{name} missing value modification"
        value_mod = x_value_args[-1].value.modifications[0]
        if isinstance(value_mod, ast.Expression):
            assert value_mod.operator in ("-", "+"), "test supports only unary +/- operator"
            assert len(value_mod.operands) == 1, "test supports only unary +/- operator"
            multiplier = -1 if value_mod.operator == "-" else 1
            value_mod_value = multiplier * value_mod.operands[0].value
        elif isinstance(value_mod, ast.Primary):
            value_mod_value = value_mod.value
        else:
            pytest.fail(f"test does not support value modification type {type(value_mod)}")
        assert value_mod_value == value, f"for {name}"


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


def test_instantiation_same_names():
    """Test instantiating a class with the same name as the enclosing class"""
    instance = parse_and_instantiate_model("TreeInTree.mo", "Tree.Tree")

    # Check symbol is present and has the correct modification
    assert "t" in instance.extends[0].symbols, "t not found in instance"
    t = instance.extends[0].symbols["t"]
    t_value_mods = get_modifiers_by_name(t, "value")
    t_value_mod = t_value_mods[-1]
    assert t_value_mod.value.modifications[-1].value == 2


def test_instantiation_modification_scope_instance_class():
    """Test that modification scopes are updated to InstanceClass"""
    ast_tree = parse_model_files("TreeInTree.mo")
    instance = tree.instantiate("Tree.Tree", ast_tree)
    assert instance is not None

    assert "t" in instance.extends[0].symbols, "t not found in instance"
    t = instance.extends[0].symbols["t"]
    for mod in t.type.symbols["Integer"].modification_environment.arguments:
        assert isinstance(mod.scope, ast.InstanceClass), "scope not InstanceClass"


def test_instantiation_modification_scope_spec_example():
    """Test scopes of modification references and values with example from spec"""

    instance = parse_and_instantiate_model("ModificationScope.mo", "B")  # noqa: F841
    pass
    # TODO: Update after new flattening is implemented (uncomment below)
    # OMC gives the following output:
    #     parameter Real R = 3.0;
    #     parameter Real a.R = 4.0;
    #     parameter Real b.R = R;
    #     parameter Real c.R = R;
    #     parameter Real d.R = d.R;
    #     constant Real d.c = 84.0;
    #     parameter Real e.R = R;
    #     parameter Real f.R = R;
    #     parameter Real g.R = R;
    #     parameter Real h.R = 42.0;
    #     parameter Real i.R = R;
    #     parameter Real j.R = 2.0;


@pytest.mark.xfail  # TODO: Figure out what we're doing with InstanceTree parent/child
def test_instantiation_tree():
    instance = parse_and_instantiate_model("InstantiationTree.mo", "TreeModel.Tree")

    # Check that we have a fully connected InstanceTree with only Instances
    non_instances = check_instance_tree_is_all_instances(instance.root)

    assert len(non_instances) == 0, f"\nFound non-instances in InstanceTree:\n{non_instances}"

    # Check that we merged the tree correctly when instantiating parents
    assert "TreeModel" in instance.root.classes
    # According to the spec, the instantiated class has root as parent
    assert "Tree" in instance.root.classes

    tree_model = instance.root.classes["TreeModel"]
    assert "TreeTypes" in tree_model.classes
    assert "TreeParts" in tree_model.classes

    tree_parts = instance.root.classes["TreeModel"].classes["TreeParts"]
    assert "Trunk" in tree_parts.classes
    assert "Branch" in tree_parts.classes
    assert "Leaf" in tree_parts.classes

    # Check that we instantiated the tree correctly
    # First check that all symbols are present
    assert sorted(instance.symbols) == ["b", "l", "w"]
    assert "t" in instance.extends[0].symbols
    # Check redeclare of w
    w = instance.symbols["w"]
    assert w.type.ast_ref.name == "Maple"
    w_mod = w.type.extends[0].symbols["Real"].modification_environment.arguments[-1]
    assert w_mod.value.component.name == "nominal"
    assert w_mod.value.modifications[0].value == pytest.approx(2.0)
    # Check redeclare of b.t
    b_t = instance.symbols["b"].type.extends[0].symbols["t"]
    assert b_t.type.ast_ref.name == "Oak"
    b_t_mod = b_t.type.extends[0].symbols["Real"].modification_environment.arguments[-1]
    assert b_t_mod.value.component.name == "nominal"
    assert b_t_mod.value.modifications[0].value == pytest.approx(1.0)
    # Check modification of l.c
    l_c = instance.symbols["l"].type.symbols["c"]
    assert l_c.type.extends[0].ast_ref.name == "Integer"
    l_c_mod = l_c.type.extends[0].symbols["Integer"].modification_environment.arguments[-1]
    assert l_c_mod.value.component.name == "value"
    assert l_c_mod.value.modifications[0].value == 1
    # Check inherited symbol t redeclare
    t = instance.extends[0].symbols["t"]
    assert t.type.ast_ref.name == "Maple"
    t_mod = t.type.extends[0].symbols["Real"].modification_environment.arguments[-1]
    assert t_mod.value.component.name == "nominal"
    assert t_mod.value.modifications[0].value == pytest.approx(2.0)


@pytest.mark.skip(reason="Only keeping first of same names not implemented yet in instantiation")
def test_instantiation_function_input_order():
    """Check that only first definitions of inputs are used in function instantiation."""
    txt = """
        package P
          function A
            input Real a;
            input Real b;
          end A;

          function B
            extends A;
            input Real b; // Should be discarded as 2nd instance of b
            input Real a; // Should be discarded as 2nd instance of a
          end B;

          function C
            input Real b;
            input Real a;
            extends A;    // Should be discarded as 2nd instances of a and b
          end C;

          function D
            input Real a; // Should be kept
            extends A;    // a should be discarded and b kept from this
            input Real b; // Should be discarded as 2nd instance of b
          end D;
        end P;
    """

    ast_tree = parser.parse(txt)

    # Our current implementation does not maintain perfect declaration order because
    # during parsing we store the declarations from the Modelica code in separate
    # symbols and extends dictionaries. To maintain order we need to store the
    # declarations in one list or ordered dictionary containing both symbols and
    # extends. The checks below are intended to fail until we have implemented this.
    # TODO: Remove above comments when we have implemented maintaining declaration order

    # TODO: Ensure the checks below are correct when we have an implementation
    instance = tree.instantiate("P.B", ast_tree)
    assert ("a", "b") == tuple(instance.extends[0].symbols)
    assert "b" not in instance.symbols
    assert "a" not in instance.symbols

    instance = tree.instantiate("P.C", ast_tree)
    assert ("b", "a") == tuple(instance.symbols)
    assert "a" not in instance.extends[0].symbols
    assert "b" not in instance.extends[0].symbols

    instance = tree.instantiate("P.D", ast_tree)
    assert "a" in instance.symbols
    assert "b" not in instance.symbols
    assert "a" not in instance.extends[0].symbols
    assert "b" in instance.extends[0].symbols


# TODO: Add additional tests for child name culling
# See in Modelica Language Spec v3.5:
# * Section 5.6.1.4 Steps of Instantiation, under "The inherited contents of the element"
# * Section 4.3 Declaration Order and Usage before Declaration
# * Chapter 12
# Tests to add:
# - function output order
# - intermixed funtion input and output declarations
# - default on one function argument but not the other with the same name - error?
# - records used as arguments to external functions
# - instances with same name but different type
# - instances with same name, same type, but different prefixes
# - instances with same name, same type, but with modifications of differing components
# - instances with same name, same type, but with modifications of same comps, different values
# - instances with same name, same type, but different redeclarations
# - ...?


def test_inheritance():
    instance = parse_and_instantiate_model("InheritanceInstantiation.mo", "C2")

    bcomp1_b = instance.symbols["bcomp1"].type.extends[0].symbols["b"]
    bcomp1_b_type = bcomp1_b.type.symbols["Integer"]
    bcomp1_b_mod = bcomp1_b_type.modification_environment.arguments[-1].value
    assert bcomp1_b_mod.component.name == "value"
    bcomp1_b_value = bcomp1_b_mod.modifications[-1].value
    assert bcomp1_b_value == 3

    bcomp3_a = instance.symbols["bcomp3"].type.extends[0].extends[0].symbols["a"]
    bcomp3_a_type = bcomp3_a.type.symbols["Real"]
    bcomp3_a_mod = bcomp3_a_type.modification_environment.arguments[-1].value
    assert bcomp3_a_mod.component.name == "value"
    bcomp3_a_value = bcomp3_a_mod.modifications[-1].value
    assert bcomp3_a_value == 1

    bcomp3_b = instance.symbols["bcomp3"].type.extends[0].extends[0].symbols["b"]
    bcomp3_b_type = bcomp3_b.type.symbols["Integer"]
    bcomp3_b_mod = bcomp3_b_type.modification_environment.arguments[-1].value
    assert bcomp3_b_mod.component.name == "value"
    bcomp3_b_value = bcomp3_b_mod.modifications[-1].value
    assert bcomp3_b_value == 2

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["bcomp1.b"].value == 3
    assert flat_tree.symbols["bcomp3.a"].value == 1
    assert flat_tree.symbols["bcomp3.b"].value == 2


def test_inheritance_resistor():
    instance = parse_and_instantiate_model("InheritanceInstantiationResistor.mo", "P.M")

    # Tests that only depend on instantiation, not flattening
    p = instance.extends[0].extends[0].symbols["p"]
    p_v = p.type.symbols["v"]
    p_v_modification = p_v.type.extends[0].symbols["Real"].modification_environment
    for arg in p_v_modification.arguments:
        if arg.value.component.name == "nominal":
            p_v_nominal = arg.value.modifications[0].value
        if arg.value.component.name == "max":
            p_v_max = arg.value.modifications[0].value
    assert p_v_nominal == 1
    assert p_v_max == 3.0

    n = instance.extends[0].extends[0].symbols["n"]
    n_v = n.type.symbols["v"]
    n_v_modification = n_v.type.extends[0].symbols["Real"].modification_environment
    for arg in n_v_modification.arguments:
        if arg.value.component.name == "nominal":
            n_v_nominal = arg.value.modifications[0].value
        if arg.value.component.name == "max":
            n_v_max = arg.value.modifications[0].value
    assert n_v_nominal == 2
    assert n_v_max == 3.0

    flat_tree = tree.flatten_instance(instance)
    assert flat_tree.symbols["p.v"].nominal == 1.0
    assert flat_tree.symbols["p.v"].max == 3.0
    assert flat_tree.symbols["n.v"].nominal == 2.0
    assert flat_tree.symbols["n.v"].max == 3.0


def test_inheritance_instantiation():
    instance = parse_and_instantiate_model("RecursiveInstantiation.mo", "M")
    ast_tree = instance.ast_ref.root

    # Test parse that b modification to redeclare A contains the p=1 modification
    b_mod = ast_tree.classes["M"].symbols["b"].class_modification
    assert b_mod is not None
    redeclare_mod = b_mod.arguments[0].value.class_modification
    assert len(redeclare_mod.arguments) == 1
    assert redeclare_mod.arguments[0].value.component.name == "p"
    assert redeclare_mod.arguments[0].value.modifications[0].value == 1

    # Test instantiation for inheritance, modifications, redeclares including scope

    assert "b" in instance.symbols
    b_type = instance.symbols["b"].type
    assert "a" in b_type.symbols
    a_extends_names = [extends.ast_ref.name for extends in b_type.symbols["a"].type.extends]
    assert "D" in a_extends_names, "model A not properly redeclared in b"
    a_extends_D_index = a_extends_names.index("D")
    a_extends_D = b_type.symbols["a"].type.extends[a_extends_D_index]
    assert "p" in a_extends_D.symbols
    p = a_extends_D.symbols["p"]
    assert "parameter" in p.prefixes
    assert isinstance(p.type, ast.InstanceClass), "b.a.p not properly instantiated"
    assert p.type.name == "E"
    assert len(p.type.extends) == 1, "b.a.p extends not properly instantiated"
    assert "Integer" in p.type.extends[0].symbols, "b.a.p incorrect type E scope"
    p_int_symbol = p.type.extends[0].symbols["Integer"]
    p_int_symbol_modification = p_int_symbol.modification_environment.arguments[0]
    p_int_symbol_value = p_int_symbol_modification.value.modifications[0].value
    assert p_int_symbol_value == 1, "b.a.p=1 modification not applied"
    assert len(a_extends_D.extends) == 1, "b.a extends not properly instantiated"
    a_extends_D_extends_C = a_extends_D.extends[0]
    assert "e" in a_extends_D_extends_C.symbols
    b_a_e = a_extends_D_extends_C.symbols["e"]
    e_type = b_a_e.type
    assert len(e_type.extends) == 1, "b.a.e extends not properly instantiated"
    assert "Real" in e_type.extends[0].symbols, "b.a.e incorrect type E scope"

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["b.a.e"].type.name == "Real"
    assert flat_tree.symbols["b.a.p"].type.name == "Integer"
    assert flat_tree.symbols["b.a.p"].value == 1


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


def test_visibility_in_instance():
    """Test visibility is set correctly in instance elements"""

    ast_tree = parse_model_files("Visibility.mo")

    instance = tree.instantiate("A", ast_tree)
    # Test visibility of the classes
    B = instance.classes["B"]
    C = instance.classes["C"]
    assert ast.Visibility.PUBLIC == instance.visibility
    assert ast.Visibility.PROTECTED == B.visibility
    assert ast.Visibility.PUBLIC == C.visibility

    # Test visibility of the symbols
    assert ast.Visibility.PROTECTED == instance.symbols["b"].visibility
    assert ast.Visibility.PUBLIC == instance.symbols["c"].visibility
    assert ast.Visibility.PUBLIC == instance.symbols["d"].visibility
    assert ast.Visibility.PROTECTED == instance.symbols["e"].visibility
    assert ast.Visibility.PROTECTED == instance.symbols["f"].visibility

    # Test that we propagate visibility to symbols in a symbol type
    for symbol in instance.symbols["b"].type.symbols.values():
        assert ast.Visibility.PROTECTED == symbol.visibility

    # Test extends visibility
    c_type_extends_B = instance.symbols["c"].type.extends[0]
    assert ast.Visibility.PROTECTED == c_type_extends_B.visibility

    # Test for protected visibility in inherited symbols in a public class
    for symbol in c_type_extends_B.symbols.values():
        assert ast.Visibility.PROTECTED == symbol.visibility


def test_extends_lookup_not_in_extended():
    """
    Check that we do not find 'model B' in the unnamed node 'A' in the
    following model. We also check that the order of inheritance does not
    matter for the error message.
    """
    txt = """
    model A
        model B
            parameter Real x = 1.0;
        end B;
        parameter Real y = 2.0;
    end A;

    model C
        extends {};
        extends {};
    end C;
    """

    for extends_1, extends_2 in [("A", "B"), ("B", "A")]:
        ast_tree = parser.parse(txt.format(extends_1, extends_2))

        with pytest.raises(tree.ModelicaSemanticError, match="Extends name B not found in scope C"):
            instance = tree.instantiate("C", ast_tree)  # noqa: F841


def test_error_extends_class_also_extended_name_simple():
    """
    Check that we are not allowed to inherit from `model B`, because a
    symbol with the same name is inherited from `model A`. We also check
    that the order of inheritance does not matter for the error message.
    """
    txt = """
    model A
        parameter Real B = 1.0;
        parameter Real y = 2.0;
    end A;

    model B
        parameter Real x = 1.0;
    end B;

    model C
        extends {};
        extends {};
    end C;
    """

    for extends_1, extends_2 in [("A", "B")]:
        ast_tree = parser.parse(txt.format(extends_1, extends_2))

        with pytest.raises(
            tree.ModelicaSemanticError,
            match="Cannot extend 'C' with 'B'; 'B' also exists in names inherited from 'A'",
        ):
            instance = tree.instantiate("C", ast_tree)  # noqa: F841


def test_error_extends_class_also_extended_name_of_self():
    """
    Check that we are not allowed to inherit from `model A`, because a
    symbol with the same name is inherited from `model A`.
    """
    txt = """
    model A
        parameter Real A = 1.0;
        parameter Real y = 2.0;
    end A;

    model C
        extends A;
    end C;
    """

    ast_tree = parser.parse(txt)

    with pytest.raises(
        tree.ModelicaSemanticError,
        match="Cannot extend 'C' with 'A'; 'A' also exists in names inherited from 'A'",
    ):
        instance = tree.instantiate("C", ast_tree)  # noqa: F841


# FIXME: Where does the spec or ModelicaCompliance say this test is valid?
@pytest.mark.xfail
def test_error_extends_class_also_extended_name_nested():
    """
    Check that we are not allowed to inherit from `model B.C`, because a
    symbol with the the name 'B' is inherited from `model A`. We also check
    that the order of inheritance does not matter for the error message.
    """
    txt = """
    model A
        parameter Real B = 1.0;
        parameter Real y = 2.0;
    end A;

    package B
        model C
            parameter Real x = 1.0;
        end C;
    end B;

    model D
        extends {};
        extends {};
    end D;
    """

    for extends_1, extends_2 in [("A", "B.C"), ("B.C", "A")]:
        ast_tree = parser.parse(txt.format(extends_1, extends_2))

        with pytest.raises(
            tree.ModelicaSemanticError,
            match="Cannot extend 'D' with 'B.C'; 'B' also exists in names inherited from 'A'",
        ):
            instance = tree.instantiate("D", ast_tree)  # noqa: F841


def test_nested_classes():
    instance = parse_and_instantiate_model("NestedClasses.mo", "C2")

    c2_v1 = instance.extends[0].symbols["v1"].type.extends[0].symbols["Real"]
    c2_v1_mod = c2_v1.modification_environment.arguments[-1].value
    assert c2_v1_mod.component.name == "nominal"
    assert c2_v1_mod.modifications[0].value == 1000

    c2_v2 = instance.extends[0].symbols["v1"].type.extends[0].symbols["Real"]
    c2_v2_mod = c2_v2.modification_environment.arguments[-1].value
    assert c2_v2_mod.component.name == "nominal"
    assert c2_v2_mod.modifications[0].value == 1000

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["v1"].nominal == 1000.0
    assert flat_tree.symbols["v2"].nominal == 1000.0


def test_nested_symbol_modifier():
    instance = parse_and_instantiate_model("NestedClasses.mo", "C3")

    c3_v1_type = instance.symbols["c"].type.extends[0].symbols["v1"].type
    c3_v1 = c3_v1_type.extends[0].symbols["Real"]
    c3_v1_mod = c3_v1.modification_environment.arguments[-1].value
    assert c3_v1_mod.component.name == "nominal"
    assert c3_v1_mod.modifications[0].value == 10

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["c.v1"].nominal == 10.0
    assert flat_tree.symbols["c.v2"].nominal == 1000.0


def test_inheritance_symbol_modifiers():
    instance = parse_and_instantiate_model("Inheritance.mo", "Sub")

    x_type = instance.extends[0].symbols["x"].type.symbols["Real"]
    x_mod = x_type.modification_environment.arguments[0]
    assert x_mod.value.component.name == "max"
    assert x_mod.value.modifications[0].value == 30.0

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["x"].max == 30.0


@pytest.mark.xfail  # "New flattening expression modification not implemented yet"
def test_extends_modification():
    instance = parse_and_instantiate_model("ExtendsModification.mo", "MainModel")

    e_HQ_H = instance.symbols["e"].type.extends[0].extends[0].symbols["HQ"].type.symbols["H"]
    H_mod = e_HQ_H.type.symbols["Real"].modification_environment.arguments[0].value
    assert H_mod.component.name == "min"
    min_mod = H_mod.modifications[0]
    assert min_mod.name == "H_b"

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["e.HQ.H"].min.name == "e.H_b"


def test_modification_typo():
    # TODO: Update when new flatting is implemented
    with open(os.path.join(MODEL_DIR, "ModificationTypo.mo"), "r") as f:
        txt = f.read()

    for c in ["Wrong1", "Wrong2"]:
        with pytest.raises(tree.ModificationTargetNotFound):
            ast_tree = parser.parse(txt)
            flat_tree = tree.flatten(ast_tree, ast.ComponentRef(name=c))

    for c in ["Good1", "Good2"]:
        ast_tree = parser.parse(txt)
        flat_tree = tree.flatten(ast_tree, ast.ComponentRef(name=c))  # noqa: F841


def test_tree_lookup():
    instance = parse_and_instantiate_model("TreeLookup.mo", "Level1.Level2.Level3.Test")

    flat_tree = tree.flatten_instance(instance)

    expect = sorted(["elem.tc.i", "elem.tc.a", "b"])
    assert expect == sorted(flat_tree.symbols.keys())


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


def test_nested_symbol_modification():
    instance = parse_and_instantiate_model("NestedSymbolModification.mo", "E")

    E_c_x = instance.extends[0].symbols["c"].type.extends[0].symbols["x"]
    mod = E_c_x.type.symbols["Real"].modification_environment.arguments[0].value
    assert mod.component.name == "nominal"
    assert mod.modifications[0].value == 2

    flat_tree = tree.flatten_instance(instance)

    assert flat_tree.symbols["c.x"].nominal == 2.0


def test_nonreplaceable_component_contains_replaceable():
    """Test that nonreplaceable components can contain replaceable components"""

    parse_and_instantiate_model("NonReplaceableContainsReplaceable.mo", "P.Test")


def test_extends_transitively_nonreplaceable_error():
    """Test that extends should fail with a replaceable in the hierarchy"""

    # TODO: Specify regex
    with pytest.raises(
        tree.ModelicaSemanticError,
        match="In P.Outer extends InnerModel, InnerModel and parents cannot be replaceable",
    ):
        parse_and_instantiate_model("NonReplaceableContainsReplaceable.mo", "P.TestFail")


def test_lookup_needs_instantiation():
    """Test that demonstrates the need for instantiation in name lookup

    Example taken from Modelica MCP 0019 discussion leading to
    Modelica Spec 3.4 Chapter 5 changes.
    """

    # Instantiation should fail without instantiation in composite name lookup
    instance = parse_and_instantiate_model("InstantiationInLookup.mo", "MyModel")
    check_redeclare_expects(instance, [redeclare_expect("x", "Real", 2.0, False)])


def test_instantiation_lookup_scope():
    """Test lookup during instantiation

    Example taken from ModelicaSpecification issues discussion on
    Modelica Spec 3.4 Chapter 5 changes.
    """

    instance = parse_and_instantiate_model("InstantiationLookupScope.mo", "M")
    x_type = instance.symbols["m"].type.symbols["x"].type
    assert x_type.name == "R"
    # Check that m.x type R is from M.B.R, not M.R
    assert len(x_type.extends), "Incorrect R scope"
    x_type_extends = x_type.extends[0]
    assert "Real" in x_type_extends.symbols
    # TODO: Fix missing dimensions for extends like in this model: `type R = Real[3]`
    # The dimensions are currently lost from the `ExtendsClause.component`.
    # See. spec 3.5 section 10.1:
    # "[Example: "The number of dimensions and the dimensions sizes are part of the
    # type, and shall be checked for example at redeclarations."
    # This should be a separate test case for the fix instead of here.
    # Section 10.1 has some test examples for this.
    # self.assertEqual(x_type_extends.symbols["Real"].dimensions[0], 3)


def test_instantiation_lookup_scope_with_modifications():
    """Test lookup during instantiation with modifications

    Example taken from ModelicaSpecification issues discussion on
    Modelica Spec 3.4 Chapter 5 changes.
    """

    instance = parse_and_instantiate_model("LexicalVsInstanceScope.mo", "P.C")
    check_redeclare_expects(instance, [redeclare_expect("n", "Integer", 3, False)])


def test_value_modification_ordering():
    """Test that value-only modifications at 3+ nesting levels pick the outermost value.

    ValF defines Real x = 1.0. ValD extends ValF(x=2.0). ValA extends ValC(ValD(x=3.0)).
    ValM extends ValA.ValD(x=4.0). The outermost value (4.0) should win.

    This is the same root cause as the redeclare ordering bug in
    test_redeclare_component_complicated but for non-redeclare value modifications.
    """

    instance = parse_and_instantiate_model(
        "NestedExtendsModification.mo", "NestedExtendsModification.ValM"
    )
    x = _find_name("x", instance, check_encapsulated=False)
    assert x is not None, "x not found in ValM instance"
    value_args = get_modifiers_by_name(x, "value")
    assert len(value_args) > 0, "x missing value modification"
    # "Last wins" — the last value modification should be 4.0 (outermost)
    value_mod = value_args[-1].value.modifications[0]
    assert isinstance(value_mod, ast.Primary)
    assert value_mod.value == 4.0, "Outermost value modification (4.0) should win"


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


def test_parse_nonexistent_file_error():
    """Test parse_file on non-existent file raises an error"""
    with pytest.raises(FileNotFoundError, match="File not found: NonExistentFile.mo"):
        parser.parse_file("NonExistentFile.mo")


def test_import():
    library_tree = parse_model_files("TreeLookup.mo", "Import.mo")

    comp_ref = ast.ComponentRef.from_string("A")
    flat_tree = tree.flatten(library_tree, comp_ref)
    expected_symbols = [
        "b.pcb.tc.a",
        "b.pcb.tc.i",
        "b.tb.b",
        "b.tb.elem.tc.a",
        "b.tb.elem.tc.i",
        "pca.tc.a",
        "pca.tc.i",
        "ta.b",
        "ta.elem.tc.a",
        "ta.elem.tc.i",
        "tce_mod.a",
        "tce_mod.i",
        "tce_mod.tcet.b",
        "tce_mod.tcet.elem.tc.a",
        "tce_mod.tcet.elem.tc.i",
    ]
    expected_symbols.sort()
    actual_symbols = sorted(flat_tree.classes["A"].symbols.keys())
    assert expected_symbols == actual_symbols
    for eqn in flat_tree.classes["A"].equations:
        if eqn.left == "tce_mod.tect.b":
            assert eqn.right.value == 4
        elif eqn.left == "b.tb.b":
            assert eqn.right.value == 3


# Import tests from the Modelica Compliance library (mostly the shouldPass=true cases)
def test_import_encapsulated():
    library_ast = parse_imports_file("EncapsulatedImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.EncapsulatedImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "a.m.x" in flat_ast.classes[model_name].symbols


def test_import_scope_type():
    library_ast = parse_imports_file("ImportScopeType.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.ImportScopeType"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "a" in flat_ast.classes[model_name].symbols
    assert "b" in flat_ast.classes[model_name].symbols
    assert "m.y" in flat_ast.classes[model_name].symbols


def test_import_qualified():
    library_ast = parse_imports_file("QualifiedImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.QualifiedImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "b.a.x" in flat_ast.classes[model_name].symbols


def test_import_renaming():
    library_ast = parse_imports_file("RenamingImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.RenamingImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "b.a.x" in flat_ast.classes[model_name].symbols


def test_import_renaming_single_definition():
    library_ast = parse_imports_file("RenamingSingleDefinitionImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.RenamingSingleDefinitionImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "b.a.x" in flat_ast.classes[model_name].symbols


def test_import_single_definition():
    library_ast = parse_imports_file("SingleDefinitionImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.SingleDefinitionImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "b.a.x" in flat_ast.classes[model_name].symbols


def test_import_unqualified():
    library_ast = parse_imports_file("UnqualifiedImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.UnqualifiedImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "b.a.x" in flat_ast.classes[model_name].symbols


def test_import_unqualified_nonconflict():
    library_ast = parse_imports_file("UnqualifiedImportNonConflict.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.UnqualifiedImportNonConflict"
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_ast = tree.flatten(library_ast, flat_class)
    assert "a.y" in flat_ast.classes[model_name].symbols


def test_import_not_inherited():
    library_ast = parse_imports_file("ExtendImport.mo")
    model_name = "ModelicaCompliance.Scoping.NameLookup.Imports.ExtendImport"
    flat_class = ast.ComponentRef.from_string(model_name)
    with pytest.raises(ast.ClassNotFoundError):
        flat_ast = tree.flatten(library_ast, flat_class)  # noqa: F841


# Tests using the Modelica Standard Library
def test_msl_opamp_units():
    """Test import from Modelica Standard Library 4.0.0 using SI.Units

    This is the simplest case found that works around current pymoca issues
    flattening MSL examples.
    """
    library_tree = parse_dir_files(
        MSL4_DIR,
        "Modelica/Icons.mo",
        "Modelica/Units.mo",
        "Modelica/Electrical/package.mo",  # to pick up SI import
        "Modelica/Electrical/Analog/Interfaces/PositivePin.mo",
        "Modelica/Electrical/Analog/Interfaces/NegativePin.mo",
        "Modelica/Electrical/Analog/Basic/OpAmp.mo",
    )
    model_name = "Modelica.Electrical.Analog.Basic.OpAmp"

    instance = tree.instantiate(model_name, library_tree)
    assert instance is not None

    # Check that we have a fully connected InstanceTree with only Instances
    non_instances = check_instance_tree_is_all_instances(instance.root)

    assert len(non_instances) == 0, f"\nFound non-instances in InstanceTree:\n{non_instances}"

    assert "i" in instance.symbols["in_p"].type.symbols
    in_p_i = instance.symbols["in_p"].type.symbols["i"]
    in_p_i_mod_args = (
        in_p_i.type.extends[0].extends[0].symbols["Real"].modification_environment.arguments
    )
    for arg in in_p_i_mod_args:
        if arg.value.component.name == "unit":
            assert arg.value.modifications[0].value == "A"
        if arg.value.component.name == "quantity":
            assert arg.value.modifications[0].value == "ElectricCurrent"

    assert "vin" in instance.symbols
    vin = instance.symbols["vin"]
    vin_mod_args = vin.type.extends[0].extends[0].symbols["Real"].modification_environment.arguments
    for arg in vin_mod_args:
        if arg.value.component.name == "unit":
            assert arg.value.modifications[0].value == "V"
        if arg.value.component.name == "quantity":
            assert arg.value.modifications[0].value == "ElectricPotential"

    flat_tree = tree.flatten_instance(instance)
    symbols = flat_tree.symbols
    assert "in_p.i" in symbols
    assert symbols["in_p.i"].unit == "A"
    assert symbols["in_p.i"].quantity == "ElectricCurrent"
    assert "vin" in symbols
    assert symbols["vin"].unit == "V"
    assert symbols["vin"].quantity == "ElectricPotential"


def test_msl3_twopin_units():
    """Test import from Modelica Standard Library 3.2.3 using SIunits

    This is a simple case that works around current pymoca issues
    flattening MSL examples.
    """
    library_tree = parse_dir_files(
        MSL3_DIR,
        "Modelica/Icons.mo",
        "Modelica/SIunits.mo",
        "Modelica/Electrical/Analog/package.mo",  # to pick up SI import
        "Modelica/Electrical/Analog/Interfaces.mo",
    )
    model_name = "Modelica.Electrical.Analog.Interfaces.TwoPort"

    instance = tree.instantiate(model_name, library_tree)
    assert instance is not None

    assert "i" in instance.symbols["p1"].type.symbols
    p1_i = instance.symbols["p1"].type.symbols["i"]
    p1_i_mod_args = (
        p1_i.type.extends[0].extends[0].symbols["Real"].modification_environment.arguments
    )
    for arg in p1_i_mod_args:
        if arg.value.component.name == "unit":
            assert arg.value.modifications[0].value == "A"
        if arg.value.component.name == "quantity":
            assert arg.value.modifications[0].value == "ElectricCurrent"

    assert "v1" in instance.symbols
    v1 = instance.symbols["v1"]
    v1_mod_args = v1.type.extends[0].extends[0].symbols["Real"].modification_environment.arguments
    for arg in v1_mod_args:
        if arg.value.component.name == "unit":
            assert arg.value.modifications[0].value == "V"
        if arg.value.component.name == "quantity":
            assert arg.value.modifications[0].value == "ElectricPotential"

    flat_tree = tree.flatten_instance(instance)
    symbols = flat_tree.symbols
    assert "p1.i" in symbols
    assert symbols["p1.i"].unit == "A"
    assert symbols["p1.i"].quantity == "ElectricCurrent"
    assert "v1" in symbols
    assert symbols["v1"].unit == "V"
    assert symbols["v1"].quantity == "ElectricPotential"


def test_msl_flange_units():
    """Test displayUnit attribute imported from MSL 4.0.0 SI.Units"""
    library_tree = parse_dir_files(
        MSL4_DIR,
        "Modelica/Icons.mo",
        "Modelica/Units.mo",
        "Modelica/Mechanics/package.mo",  # to pick up SI import
        "Modelica/Mechanics/Rotational/Interfaces/Flange.mo",
        "Modelica/Mechanics/Rotational/Interfaces/Flange_a.mo",
        "Modelica/Mechanics/Rotational/Interfaces/PartialAbsoluteSensor.mo",
    )
    model_name = "Modelica.Mechanics.Rotational.Interfaces.PartialAbsoluteSensor"
    instance = tree.instantiate(model_name, library_tree)
    assert instance is not None

    assert "flange" in instance.symbols
    phi = instance.symbols["flange"].type.extends[0].symbols["phi"]
    phi_mod_args = phi.type.extends[0].symbols["Real"].modification_environment.arguments
    for arg in phi_mod_args:
        if arg.value.component.name == "unit":
            assert arg.value.modifications[0].value == "rad"
        if arg.value.component.name == "displayUnit":
            assert arg.value.modifications[0].value == "deg"
        if arg.value.component.name == "quantity":
            assert arg.value.modifications[0].value == "Angle"

    flat_tree = tree.flatten_instance(instance)
    symbols = flat_tree.symbols
    assert "flange.phi" in symbols
    assert symbols["flange.phi"].unit == "rad"
    assert symbols["flange.phi"].displayUnit == "deg"
    assert symbols["flange.phi"].quantity == "Angle"


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


def test_derived_type_value_modification():
    """Test modifying the value of a derived type"""
    txt = """
        package A
            type X = Integer;
            type Y = X; /* Derived type */
            model B
                Y y = 1; /* Modification 1 */
            end B;
            model C
                B c(y = 2); /* Modification 2 */
            end C;
            model D = C(c.y = 3); /* Modification 3 */
            model E
                D d(c.y = 4); /* Modification 4 */
            end E;
        end A;
    """
    ast_tree = parser.parse(txt)
    instance = tree.instantiate("A.D", ast_tree)
    assert instance is not None
    c_y = (
        instance.extends[0]
        .symbols["c"]
        .type.symbols["y"]
        .type.extends[0]
        .extends[0]
        .symbols["Integer"]
    )
    c_y_mod = c_y.modification_environment.arguments[-1]
    assert c_y_mod.value.component.name == "value"
    assert c_y_mod.value.modifications[0].value == 3

    instance = tree.instantiate("A.E", ast_tree)
    d_c_y = (
        instance.symbols["d"]
        .type.extends[0]
        .symbols["c"]
        .type.symbols["y"]
        .type.extends[0]
        .extends[0]
        .symbols["Integer"]
    )
    d_c_y_mod = d_c_y.modification_environment.arguments[-1]
    assert d_c_y_mod.value.component.name == "value"
    assert d_c_y_mod.value.modifications[0].value == 4

    flat_tree = tree.flatten_instance(instance)
    # TODO: Uncomment when modifications are added as equations
    # self.assertEqual(flat_tree.equations[0].left.name, "d.c.y")
    # self.assertEqual(flat_tree.equations[0].right.value, 4)
    # For now, the symbol value attribute is assigned the modification value
    assert "d.c.y" in flat_tree.symbols
    assert flat_tree.symbols["d.c.y"].value == 4


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


def test_parse_cache_hit(caplog):
    """Test caching of models"""

    txt = """
        model A
          parameter Real x, y;
        equation
          der(y) = x;
        end A;
    """

    with modify_version(WorkDirState.CLEAN), tempfile.TemporaryDirectory() as tmpdirname:
        full_db_path = Path(tmpdirname) / DEFAULT_MODEL_CACHE_DB

        # The cache database should not exist yet
        assert not full_db_path.exists()

        _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

        # And now the database _should exist_, and we check its contents
        # where we expect to find a single cached entry
        assert full_db_path.exists()

        conn = sqlite3.connect(full_db_path)
        cursor = conn.cursor()

        cursor.execute("SELECT value FROM metadata WHERE key='created_at'")
        first_created_at = int(cursor.fetchone()[0])

        cursor.execute("SELECT last_hit FROM models")
        first_hit_time = cursor.fetchone()[0]

        cursor.execute("SELECT COUNT(*) FROM models")
        assert cursor.fetchone()[0] == 1

        # Check that we get log messages saying the cache entry was found
        # We also force an update to the cache hit time
        with caplog.at_level(logging.DEBUG, logger="pymoca"):
            _ = parser.parse(txt, model_cache_folder=Path(tmpdirname), always_update_last_hit=True)
            assert any(") found in cache" in record.message for record in caplog.records)

        cursor.execute("SELECT value FROM metadata WHERE key='created_at'")
        second_created_at = int(cursor.fetchone()[0])

        cursor.execute("SELECT last_hit FROM models")
        second_hit_time = cursor.fetchone()[0]

        # Check that the created_at time was _not_ updated, i.e. the
        # database was not recreated for some reason.
        assert first_created_at == second_created_at

        # Check that, if we parse it _again_, the `last_hit` updates
        assert second_hit_time > first_hit_time

        # Check that there's still only one model in the cache
        cursor.execute("SELECT COUNT(*) FROM models")
        assert cursor.fetchone()[0] == 1

        cursor.close()
        conn.close()


def test_parse_cache_purge():
    """Test that models that have not been hit in N days are purged"""

    model_a = """
        model A
          parameter Real x, y;
        equation
          der(y) = x;
        end A;
    """

    model_b = """
        model B
          parameter Real x, y;
        equation
          der(y) = x;
        end B;
    """

    with modify_version(WorkDirState.CLEAN), tempfile.TemporaryDirectory() as tmpdirname:
        full_db_path = Path(tmpdirname) / DEFAULT_MODEL_CACHE_DB

        # Parse the models to add them to the cache
        for txt in [model_a, model_b]:
            _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

        conn = sqlite3.connect(full_db_path)
        cursor = conn.cursor()

        # Check that the models are in the cache
        cursor.execute("SELECT COUNT(*) FROM models")
        assert cursor.fetchone()[0] == 2

        cursor.execute("SELECT value FROM metadata WHERE key='last_prune'")
        first_prune_time = int(cursor.fetchone()[0])

        cursor.execute("SELECT value FROM metadata WHERE key='created_at'")
        first_created_at = int(cursor.fetchone()[0])

        # Reimport the module to force a cache purge check, but with an
        # expiration time such that the models should not be purged
        import importlib

        importlib.reload(parser)

        _ = parser.parse(
            model_b,
            model_cache_folder=Path(tmpdirname),
            cache_expiration_days=1,
        )

        cursor.execute("SELECT COUNT(*) FROM models")
        assert cursor.fetchone()[0] == 2

        # Reimport the module again, but now we force a purge by setting
        # expiration to zero
        importlib.reload(parser)

        _ = parser.parse(
            model_b,
            model_cache_folder=Path(tmpdirname),
            cache_expiration_days=0,
        )

        cursor.execute("SELECT value FROM metadata WHERE key='last_prune'")
        second_prune_time = int(cursor.fetchone()[0])

        cursor.execute("SELECT value FROM metadata WHERE key='created_at'")
        second_created_at = int(cursor.fetchone()[0])

        # Check that the other model has been purged from the cache.
        cursor.execute("SELECT COUNT(*) FROM models")
        assert cursor.fetchone()[0] == 1

        # Check that the last prune time was updated
        assert second_prune_time > first_prune_time

        # And that the creation time was not
        assert first_created_at == second_created_at

        cursor.close()
        conn.close()


def test_dirty_no_caching():
    """Test cache and cache creation bypass if working directory is dirty"""

    txt = """
        model A
          parameter Real x, y;
        equation
          der(y) = x;
        end A;
    """

    with modify_version(WorkDirState.DIRTY), tempfile.TemporaryDirectory() as tmpdirname:
        full_db_path = Path(tmpdirname) / DEFAULT_MODEL_CACHE_DB

        # The cache database should not exist yet
        assert not full_db_path.exists()

        _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

        # And now the database should not exist
        assert not full_db_path.exists()


def test_unpickling_error(caplog):
    """Test that we can handle unpickling errors, and then recreate the cache entry"""

    txt = """
        model A
          parameter Real x, y;
        equation
          der(y) = x;
        end A;
    """

    with modify_version(WorkDirState.CLEAN), tempfile.TemporaryDirectory() as tmpdirname:
        full_db_path = Path(tmpdirname) / DEFAULT_MODEL_CACHE_DB

        # The cache database should not exist yet
        assert not full_db_path.exists()

        _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

        assert full_db_path.exists()

        # Modify the single entry in the cache to make it unpickleable
        conn = sqlite3.connect(full_db_path)
        cursor = conn.cursor()

        cursor.execute("SELECT txt_hash FROM models")
        txt_hash = cursor.fetchone()[0]
        cursor.execute("UPDATE models SET data = ? WHERE txt_hash = ?", (b"not a pickle", txt_hash))
        conn.commit()

        cursor.close()
        conn.close()

        # Check that we get log messages saying the cache entry is corrupt
        with caplog.at_level(logging.WARNING, logger="pymoca"):
            _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

            assert any("failed to unpickle" in record.message for record in caplog.records)
            n_warnings = len([r for r in caplog.records if r.levelno >= logging.WARNING])

            # Check that we get no additional warnings when unpickling again
            _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))
            assert len([r for r in caplog.records if r.levelno >= logging.WARNING]) == n_warnings


def test_incorrect_table_layout(caplog):
    """Test that a corrupt cache file is ignored"""

    txt = """
        model A
          parameter Real x, y;
        equation
          der(y) = x;
        end A;
    """

    with modify_version(WorkDirState.CLEAN), tempfile.TemporaryDirectory() as tmpdirname:
        full_db_path = Path(tmpdirname) / DEFAULT_MODEL_CACHE_DB

        # The cache database should not exist yet
        assert not full_db_path.exists()

        # Create a database with incorrectly structured tables
        conn = sqlite3.connect(full_db_path)
        cursor = conn.cursor()

        dummy_table_str = """
            CREATE TABLE {} (
                wrong_key TEXT,
                wrong_value TEXT,
                PRIMARY KEY (wrong_key)
            )
        """

        cursor.execute(dummy_table_str.format("models"))
        cursor.execute(dummy_table_str.format("metadata"))

        conn.close()

        # And now the database should exist
        assert full_db_path.exists()

        # Check that we get log messages saying the layout is incorrect
        with caplog.at_level(logging.WARNING, logger="pymoca"):
            _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

            warning_messages = [r.message for r in caplog.records if r.levelno >= logging.WARNING]
            assert any("Model text cache table layout didn't match" in m for m in warning_messages)
            assert any("Metadata table layout didn't match" in m for m in warning_messages)


def test_corrupt_cache_file(caplog):
    """Test that a corrupt cache file is ignored"""

    txt = """
        model A
          parameter Real x, y;
        equation
          der(y) = x;
        end A;
    """

    with modify_version(WorkDirState.CLEAN), tempfile.TemporaryDirectory() as tmpdirname:
        full_db_path = Path(tmpdirname) / DEFAULT_MODEL_CACHE_DB

        # The cache database should not exist yet
        assert not full_db_path.exists()

        # Create a corrupt cache file
        with open(full_db_path, "w") as f:
            f.write("This is not a valid SQLite database file")

        with caplog.at_level(logging.WARNING, logger="pymoca"):
            _ = parser.parse(txt, model_cache_folder=Path(tmpdirname))

            assert any(
                "Model cache database is corrupt" in record.message for record in caplog.records
            )


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
