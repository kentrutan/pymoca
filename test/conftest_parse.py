"""
Shared utilities and constants for parse-related tests.
"""

import os
import pickle
import sys
import time
from collections import namedtuple

from pymoca import ast
from pymoca import parser
from pymoca import tree
from pymoca.tree._name_lookup import _find_name

import pytest

MY_DIR = os.path.dirname(os.path.realpath(__file__))
MODEL_DIR = os.path.join(MY_DIR, "models")
COMPLIANCE_DIR = os.path.join(MY_DIR, "libraries", "Modelica-Compliance", "ModelicaCompliance")
IMPORTS_DIR = os.path.join(COMPLIANCE_DIR, "Scoping", "NameLookup", "Imports")
MSL3_DIR = os.path.join(MY_DIR, "libraries", "MSL-3.2.3")
MSL4_DIR = os.path.join(MY_DIR, "libraries", "MSL-4.0.x")

redeclare_expect = namedtuple("redeclare_expect", ["name", "type", "value", "replaceable"])


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
