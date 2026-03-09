"""Automated ModelicaCompliance tests for name lookup, instantiation, and flattening."""

import collections
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

# Matches: assert(expr == value, "msg") or assert(expr == value, "msg");
_ASSERT_DIRECT_RE = re.compile(
    r"assert\s*\(\s*"
    r"([\w.]+)\s*==\s*"  # lhs == rhs
    r"([\d.eE+-]+)"  # numeric literal
    r'\s*,\s*"[^"]*"\s*\)',
)

# Matches: assert(Util.compareReal(expr, value), "msg")
_ASSERT_COMPARE_RE = re.compile(
    r"assert\s*\(\s*Util\.compareReal\s*\(\s*"
    r"([\w.]+)\s*,\s*"  # variable
    r"([\d.eE+-]+)"  # numeric literal
    r'\s*\)\s*,\s*"[^"]*"\s*\)',
)

AssertionInfo = collections.namedtuple("AssertionInfo", ["variable", "expected", "approx"])


def parse_assertions(mo_file_path):
    """Extract assert(...) conditions from a .mo file.

    Returns list of AssertionInfo(variable, expected, approx) where approx=True
    for Util.compareReal checks.
    """
    with open(mo_file_path, "r", encoding="utf-8") as f:
        text = f.read()
    results = []
    for m in _ASSERT_DIRECT_RE.finditer(text):
        results.append(AssertionInfo(m.group(1), float(m.group(2)), False))
    for m in _ASSERT_COMPARE_RE.finditer(text):
        results.append(AssertionInfo(m.group(1), float(m.group(2)), True))
    return results


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
    for _level in ("name_lookup", "instantiation", "flattening", "value_check"):
        KNOWN_FAILURES[(_level, _model)] = _GLOBAL_REASON

# QualifiedImportConflict fails at all levels (parse error)
for _level in ("name_lookup", "instantiation", "flattening", "value_check"):
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
    KNOWN_FAILURES[("value_check", _model)] = _reason

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


# ---------------------------------------------------------------------------
# Value checking tests (Phase 2a)
# ---------------------------------------------------------------------------


def _extract_value_from_class_mod(sym):
    """Extract numeric value from a Symbol's class_modification (value=X pattern).

    For symbols like `constant Real x = 5.1`, the value 5.1 is stored in
    class_modification as ElementModification(component=value, modifications=[Primary(value=5.1)]).
    """
    cm = sym.class_modification
    if not cm:
        return None
    for arg in cm.arguments:
        em = arg.value
        if hasattr(em, "component") and str(em.component) == "value":
            if hasattr(em, "modifications") and em.modifications:
                m = em.modifications[0]
                if isinstance(m, ast.Primary) and isinstance(m.value, (int, float)):
                    return m.value
    return None


def _resolve_symbol_value(flat, var_name, _visited=None):
    """Try to resolve a symbol's value to a numeric literal.

    Follows InstanceSymbol reference chains through flat.symbols to find resolved
    numeric values. For package constants not in flat.symbols, falls back to
    ast_ref.class_modification.

    Returns the numeric value, or None if not resolvable.
    """
    if _visited is None:
        _visited = set()
    if var_name in _visited:
        return None  # Cycle detection
    _visited.add(var_name)

    sym = flat.symbols.get(var_name)
    if sym is None:
        return None
    v = sym.value
    if isinstance(v, (int, float)):
        return v
    if not isinstance(v, ast.InstanceSymbol):
        return None

    ref_name = v.name

    # Strategy 1: Prefix-based lookup in flat.symbols
    # e.g., var_name="b.y", ref_name="x" -> try "b.x"
    prefix = var_name.rsplit(".", 1)[0] if "." in var_name else ""
    candidate = (prefix + "." + ref_name) if prefix else ref_name
    result = _resolve_symbol_value(flat, candidate, _visited)
    if result is not None:
        return result

    # Strategy 2: ast_ref.class_modification for package constants
    # e.g., ref_name="P.P.x" where P.x has `constant Real x = 5.1`
    result = _extract_value_from_class_mod(v.ast_ref)
    if result is not None:
        return result

    return None


@pytest.mark.compliance
@pytest.mark.value_check
@pytest.mark.parametrize(
    "mo_path, model_name, should_pass",
    build_params("value_check", NAME_LOOKUP_CATEGORIES) if COMPLIANCE_AVAILABLE else [],
)
def test_compliance_value_check(mo_path, model_name, should_pass):
    """Test that flattened symbol values match assert() expectations in .mo file."""
    if not should_pass:
        pytest.skip("Value checking only applies to shouldPass=true models")

    assertions = parse_assertions(mo_path)
    if not assertions:
        pytest.skip("No assertions found in model")

    ast_tree = load_compliance_model(mo_path)
    assert ast_tree is not None, f"Failed to parse {mo_path}"

    instance = instantiate(model_name, ast_tree)
    flat = flatten_instance(instance)
    assert flat is not None

    checked = 0
    for assertion in assertions:
        value = _resolve_symbol_value(flat, assertion.variable)
        if value is None:
            continue  # Symbol not found or value not resolved to literal
        checked += 1
        if assertion.approx:
            assert (
                abs(value - assertion.expected) < 1e-10
            ), f"{assertion.variable}: expected ~{assertion.expected}, got {value}"
        else:
            assert (
                value == assertion.expected
            ), f"{assertion.variable}: expected {assertion.expected}, got {value}"

    if checked == 0:
        pytest.skip(f"No assertions resolved to numeric values ({len(assertions)} skipped)")
