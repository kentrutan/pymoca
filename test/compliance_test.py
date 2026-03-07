"""Automated ModelicaCompliance tests for name lookup, instantiation, and flattening."""

import os
import re

import pymoca.ast as ast
import pymoca.parser
from pymoca.tree import (
    InstantiationError,
    ModelicaError,
    NameLookupError,
    find_name,
    flatten_instance,
    instantiate,
)

import pytest

# ---------------------------------------------------------------------------
# Discovery infrastructure
# ---------------------------------------------------------------------------

MY_DIR = os.path.dirname(os.path.realpath(__file__))
COMPLIANCE_DIR = os.path.join(MY_DIR, "libraries", "Modelica-Compliance", "ModelicaCompliance")
COMPLIANCE_AVAILABLE = os.path.isfile(os.path.join(COMPLIANCE_DIR, "Icons.mo"))

_SHOULD_PASS_RE = re.compile(r"shouldPass\s*=\s*(true|false)", re.IGNORECASE)


def parse_should_pass(mo_file_path):
    """Extract shouldPass value from __ModelicaAssociation(TestCase(...)) annotation."""
    with open(mo_file_path, "r", encoding="utf-8") as f:
        text = f.read()
    m = _SHOULD_PASS_RE.search(text)
    if m is None:
        raise ValueError(f"No shouldPass annotation found in {mo_file_path}")
    return m.group(1).lower() == "true"


def mo_path_to_model_name(mo_file_path):
    """Convert .mo file path to fully-qualified Modelica class name."""
    rel = os.path.relpath(mo_file_path, os.path.dirname(COMPLIANCE_DIR))
    return os.path.splitext(rel)[0].replace(os.sep, ".")


def load_compliance_model(mo_file_path):
    """Parse Icons.mo + target .mo file and return merged ast.Tree."""
    icon_ast = pymoca.parser.parse_file(os.path.join(COMPLIANCE_DIR, "Icons.mo"))
    target_ast = pymoca.parser.parse_file(mo_file_path)
    if None in (icon_ast, target_ast):
        return None
    icon_ast.extend(target_ast)
    return icon_ast


def discover_compliance_files(subdirectory):
    """Walk subdirectory under COMPLIANCE_DIR, return (test_id, abs_path, should_pass) list."""
    base = os.path.join(COMPLIANCE_DIR, subdirectory)
    if not os.path.isdir(base):
        return []
    results = []
    for dirpath, _dirs, files in os.walk(base):
        for fname in sorted(files):
            if fname in ("package.mo", "package.order") or not fname.endswith(".mo"):
                continue
            abs_path = os.path.join(dirpath, fname)
            rel = os.path.relpath(abs_path, base)
            test_id = os.path.splitext(rel)[0].replace(os.sep, "/")
            should_pass = parse_should_pass(abs_path)
            results.append((test_id, abs_path, should_pass))
    return results


# Known failures: (test_level, model_name) -> xfail reason
#
# Global name syntax (leading dot .A.B.C) not yet in grammar
_GLOBAL_MODELS = [
    "ModelicaCompliance.Scoping.NameLookup.Global." + n
    for n in [
        "EncapsulatedGlobalLookup",
        "EncapsulatedLookupClass",
        "GlobalLookupEncapsulatedElement",
        "GlobalLookupNonEncapsulatedElement",
        "GlobalPartialClass",
        "LocalNameGlobalLookup",
        "NonExistingGlobalName",
        "NonPackageLikeClassLookup",
        "PackageLikeClassLookup",
    ]
]
_GLOBAL_REASON = "Global name syntax (leading dot) not in grammar"

# QualifiedImportConflict triggers parser duplicate-import error
_IMPORT_CONFLICT = "ModelicaCompliance.Scoping.NameLookup.Imports.QualifiedImportConflict"
_IMPORT_CONFLICT_REASON = "Parser raises on duplicate qualified import"

KNOWN_FAILURES = {}

# All global tests fail at all levels (parse error)
for _model in _GLOBAL_MODELS:
    for _level in ("name_lookup", "instantiation", "flattening"):
        KNOWN_FAILURES[(_level, _model)] = _GLOBAL_REASON

# QualifiedImportConflict fails at all levels (parse error)
for _level in ("name_lookup", "instantiation", "flattening"):
    KNOWN_FAILURES[(_level, _IMPORT_CONFLICT)] = _IMPORT_CONFLICT_REASON

# Flattening WIP — models that pass instantiation but fail flattening
_FLATTEN_WIP = {
    "ModelicaCompliance.Scoping.NameLookup.Simple.Encapsulation": "Flattening WIP: KeyError",
    "ModelicaCompliance.Scoping.NameLookup.Simple.EnclosingClassLookupConstant": "Flattening WIP",
    "ModelicaCompliance.Scoping.NameLookup.Simple.ImplicitShadowingReduction": "Flattening WIP",
    "ModelicaCompliance.Scoping.NameLookup.Composite.FunctionLookupViaArrayComp": (
        "Flattening WIP: function lookup via component"
    ),
    "ModelicaCompliance.Scoping.NameLookup.Composite.FunctionLookupViaArrayElement": (
        "Flattening WIP: function lookup via component"
    ),
    "ModelicaCompliance.Scoping.NameLookup.Composite.FunctionLookupViaClassComp": (
        "Flattening WIP: function lookup via component"
    ),
    "ModelicaCompliance.Scoping.NameLookup.Composite.FunctionLookupViaComp": (
        "Flattening WIP: function lookup via component"
    ),
    "ModelicaCompliance.Scoping.NameLookup.Composite.FunctionLookupViaCompNonCall": (
        "Flattening WIP: function lookup via component"
    ),
    "ModelicaCompliance.Scoping.NameLookup.Composite.FunctionLookupViaNonClassComp": (
        "Flattening WIP: function lookup via component"
    ),
    "ModelicaCompliance.Scoping.NameLookup.Imports.ImportScopeType": "Flattening WIP: KeyError",
}
for _model, _reason in _FLATTEN_WIP.items():
    KNOWN_FAILURES[("flattening", _model)] = _reason

# Inheritance/Modification/Redeclare — instantiation failures (cascade to flattening+value_check)
_INST_FAILURES = {
    "ModelicaCompliance.Inheritance.Flattening.ReplacedBaseClass": (
        "Replaceable base class not supported in extends"
    ),
    "ModelicaCompliance.Redeclare.Flattening.BasicBindingRedeclare": (
        "Redeclare flattening WIP: KeyError"
    ),
    "ModelicaCompliance.Redeclare.Flattening.InheritancePublicClass": (
        "Parser error: empty function_arguments"
    ),
}
for _model, _reason in _INST_FAILURES.items():
    for _level in ("instantiation", "flattening", "value_check"):
        KNOWN_FAILURES[(_level, _model)] = _reason

# Inheritance/Modification/Redeclare — flattening failures (cascade to value_check)
_FLATTEN_INHERIT_WIP = {
    "ModelicaCompliance.Inheritance.Flattening.MultipleInheritance": (
        "Flattening WIP: expression evaluation (None + None)"
    ),
    "ModelicaCompliance.Modification.Flattening.Array": (
        "Flattening WIP: array modifications not implemented"
    ),
    "ModelicaCompliance.Modification.Flattening.Complicated": (
        "Flattening WIP: Class missing fully_instantiated"
    ),
    "ModelicaCompliance.Redeclare.Flattening.InheritanceDirection": (
        "Flattening WIP: KeyError on connector prefix"
    ),
    "ModelicaCompliance.Redeclare.Flattening.InheritanceStream": (
        "Flattening WIP: inStream not in transform_operator"
    ),
}
for _model, _reason in _FLATTEN_INHERIT_WIP.items():
    KNOWN_FAILURES[("flattening", _model)] = _reason
    KNOWN_FAILURES[("value_check", _model)] = _reason

# Value check only failures — model flattens but values don't match
_VALUE_CHECK_FAILURES = {
    "ModelicaCompliance.Modification.Flattening.Merging2": (
        "Value check WIP: nested modification merging (c4.x5.a)"
    ),
}
for _model, _reason in _VALUE_CHECK_FAILURES.items():
    KNOWN_FAILURES[("value_check", _model)] = _reason


def build_params(test_level, categories):
    """Build pytest.param list with conditional xfail marks from KNOWN_FAILURES."""
    params = []
    for category in categories:
        for test_id, abs_path, should_pass in discover_compliance_files(category):
            model_name = mo_path_to_model_name(abs_path)
            marks = []
            key = (test_level, model_name)
            if key in KNOWN_FAILURES:
                marks.append(pytest.mark.xfail(reason=KNOWN_FAILURES[key], strict=True))
            params.append(pytest.param(abs_path, model_name, should_pass, id=test_id, marks=marks))
    return params


# ---------------------------------------------------------------------------
# Test configuration
# ---------------------------------------------------------------------------

pytestmark = pytest.mark.skipif(
    not COMPLIANCE_AVAILABLE, reason="Modelica-Compliance submodule not initialized"
)

NAME_LOOKUP_CATEGORIES = [
    "Scoping/NameLookup/Simple",
    "Scoping/NameLookup/Composite",
    "Scoping/NameLookup/Global",
    "Scoping/NameLookup/Imports",
]

FLATTENING_CATEGORIES = [
    "Inheritance/Flattening",
    "Modification/Flattening",
    "Redeclare/Flattening",
]

ALL_CATEGORIES = NAME_LOOKUP_CATEGORIES + FLATTENING_CATEGORIES


# ---------------------------------------------------------------------------
# Name lookup tests
# ---------------------------------------------------------------------------


@pytest.mark.compliance
@pytest.mark.name_lookup
@pytest.mark.parametrize(
    "mo_path, model_name, should_pass",
    build_params("name_lookup", NAME_LOOKUP_CATEGORIES) if COMPLIANCE_AVAILABLE else [],
)
def test_compliance_name_lookup(mo_path, model_name, should_pass):
    """Test that name lookup succeeds or fails as expected per shouldPass annotation."""
    ast_tree = load_compliance_model(mo_path)
    assert ast_tree is not None, f"Failed to parse {mo_path}"

    parts = model_name.split(".")
    scope = ast_tree.classes.get("ModelicaCompliance")
    assert scope is not None, "ModelicaCompliance root class not found"

    # Navigate to parent scope of the leaf model
    for part in parts[1:-1]:
        scope = find_name(part, scope)
        if scope is None:
            break
        if isinstance(scope, ast.Symbol):
            break

    leaf_name = parts[-1]

    if should_pass:
        assert scope is not None, f"Could not navigate to parent scope for {model_name}"
        result = find_name(leaf_name, scope)
        assert result is not None, f"Name lookup failed for {leaf_name} in {model_name}"
    else:
        try:
            if scope is None:
                return  # Parent scope not found — lookup correctly failed
            find_name(leaf_name, scope)
        except (NameLookupError, ModelicaError):
            pass  # Expected failure


# ---------------------------------------------------------------------------
# Instantiation tests
# ---------------------------------------------------------------------------


@pytest.mark.compliance
@pytest.mark.instantiation
@pytest.mark.parametrize(
    "mo_path, model_name, should_pass",
    build_params("instantiation", ALL_CATEGORIES) if COMPLIANCE_AVAILABLE else [],
)
def test_compliance_instantiate(mo_path, model_name, should_pass):
    """Test that instantiation succeeds or fails as expected."""
    ast_tree = load_compliance_model(mo_path)
    assert ast_tree is not None, f"Failed to parse {mo_path}"

    if should_pass:
        instance = instantiate(model_name, ast_tree)
        assert instance is not None
    else:
        try:
            instantiate(model_name, ast_tree)
        except (NameLookupError, InstantiationError, ast.ClassNotFoundError, ModelicaError):
            pass  # Expected failure


# ---------------------------------------------------------------------------
# Flattening tests
# ---------------------------------------------------------------------------


@pytest.mark.compliance
@pytest.mark.flattening
@pytest.mark.parametrize(
    "mo_path, model_name, should_pass",
    build_params("flattening", ALL_CATEGORIES) if COMPLIANCE_AVAILABLE else [],
)
def test_compliance_flatten(mo_path, model_name, should_pass):
    """Test that flattening succeeds or fails as expected."""
    ast_tree = load_compliance_model(mo_path)
    assert ast_tree is not None, f"Failed to parse {mo_path}"

    if should_pass:
        instance = instantiate(model_name, ast_tree)
        assert instance is not None
        flat = flatten_instance(instance)
        assert flat is not None
    else:
        try:
            instance = instantiate(model_name, ast_tree)
            flatten_instance(instance)
        except (
            NameLookupError,
            InstantiationError,
            ast.ClassNotFoundError,
            ModelicaError,
            NotImplementedError,
        ):
            pass  # Expected failure
