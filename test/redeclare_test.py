#!/usr/bin/env python
"""
Tests for redeclaration semantics.
"""

import os

from conftest_parse import (
    MODEL_DIR,
    check_redeclare_expects,
    parse_and_flatten_model,
    parse_and_instantiate_model,
    parse_model_files,
    redeclare_expect,
)

from pymoca import parser
from pymoca import tree

import pytest


def test_flattening_redeclare_class_extends():
    """Test redeclare class extends construct"""

    parse_and_instantiate_model("RedeclareClassExtends.mo", "P.TestOK")

    flat = parse_and_flatten_model("RedeclareClassExtends.mo", "P.TestOK")
    assert "ma.X" in flat.symbols
    assert "ma.T" in flat.symbols
    assert "eta" in flat.symbols

    eq_map = {eq.left.name: eq.right for eq in flat.equations if hasattr(eq, "left")}
    assert "ma.X" in eq_map
    assert [v.value for v in eq_map["ma.X"].values] == [0, 1]
    assert "eta" in eq_map
    assert eq_map["eta"].operator == "P.MoistAir.dynamicViscosity"


@pytest.mark.xfail(
    reason="undefined array-dimension constants are not yet detected (MLS §10.1)",
)
def test_undefined_array_dimension_constant():
    """A constant used as an array dimension must be evaluable (MLS §10.1).
    P.TestFail2 isolates the nX-undefined intent of MLS §7.3.1 (MoistAir2) without
    triggering the §5.3.2 partial-class lookup error that masks it in P.TestFail."""
    with pytest.raises(tree.ModelicaSemanticError):
        parse_and_instantiate_model("RedeclareClassExtends.mo", "P.TestFail2")


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

    # TODO: Update when value/type compatibility checking is added to flattening
    # The redeclare clears the original value, so all flattened values are None.
    # Type compatibility errors (e.g. Boolean x = true redeclared as Integer)
    # should be caught in a future type-checking pass.

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

    # Flattening succeeds (no type errors raised yet)
    flat = tree.flatten_instance(instance)
    assert len(flat.symbols) == 12


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
        _ = tree.instantiate(ast_tree, "A")


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
        _ = tree.instantiate(ast_tree, "A")


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
        instance = tree.instantiate(ast_tree, "E")  # noqa: F841


def test_redeclare_nonreplaceable():
    ast_tree = parser.parse_file(os.path.join(MODEL_DIR, "RedeclareNonReplaceable.mo"))

    with pytest.raises(tree.ModelicaSemanticError, match="Redeclaring D.C that is not replaceable"):
        instance = tree.instantiate(ast_tree, "E")  # noqa: F841


def test_redeclare_nested():
    with pytest.raises(parser.ModelicaSyntaxError, match="missing '=' at '.'"):
        _ = parse_model_files("RedeclareNestedClass.mo.fail_parse")


# --- Prefix check/preserve tests (MLS 7.3) ---


def _flatten_inline(txt, model_name):
    ast_tree = parser.parse(txt)
    instance = tree.instantiate(ast_tree, model_name)
    return tree.flatten_instance(instance)


def test_redeclare_connector_prefix_preserved_from_original():
    """flow prefix from original is preserved when redeclare omits it."""
    flat = _flatten_inline(
        """
    connector C
        replaceable flow Real x;
    end C;
    connector D
        extends C(redeclare Real x);
    end D;""",
        "D",
    )
    assert "flow" in flat.symbols["x"].prefixes


def test_redeclare_connector_prefix_matches_original():
    """Redeclare that repeats the original flow prefix is accepted."""
    flat = _flatten_inline(
        """
    connector C
        replaceable flow Real x;
    end C;
    connector D
        extends C(redeclare flow Real x);
    end D;""",
        "D",
    )
    assert "flow" in flat.symbols["x"].prefixes


def test_redeclare_connector_prefix_mismatch_raises():
    """Adding flow to a non-flow original raises ModelicaSemanticError."""
    with pytest.raises(
        tree.ModelicaSemanticError,
        match=r"Redeclare of 'C\.x' changes connector prefix from None to 'flow'",
    ):
        _flatten_inline(
            """
    connector C
        replaceable Real x;
    end C;
    connector D
        extends C(redeclare flow Real x);
    end D;""",
            "D",
        )


def test_redeclare_variability_strengthen_accepted():
    """Redeclare may strengthen variability from continuous to parameter."""
    flat = _flatten_inline(
        """
    model Base
        replaceable Real x = 0.0;
    end Base;
    model Derived
        extends Base(redeclare parameter Real x = 1.0);
    end Derived;""",
        "Derived",
    )
    assert "parameter" in flat.symbols["x"].prefixes


def test_redeclare_variability_loosen_raises():
    """Explicit weakening of variability from parameter to discrete raises."""
    with pytest.raises(
        tree.ModelicaSemanticError,
        match=r"Redeclare of 'Base\.x' loosens variability from 'parameter' to 'discrete'",
    ):
        _flatten_inline(
            """
    model Base
        replaceable parameter Real x = 0.0;
    end Base;
    model Derived
        extends Base(redeclare discrete Real x = 1.0);
    end Derived;""",
            "Derived",
        )


def test_redeclare_variability_omit_preserves():
    """Omitting variability in a redeclare preserves the original prefix (MLS 7.3)."""
    flat = _flatten_inline(
        """
    model Base
        replaceable parameter Real x = 0.0;
    end Base;
    model Derived
        extends Base(redeclare Real x = 1.0);
    end Derived;""",
        "Derived",
    )
    assert "parameter" in flat.symbols["x"].prefixes


def test_redeclare_causality_mismatch_raises():
    """Redeclare that changes causality from input to output raises."""
    with pytest.raises(
        tree.ModelicaSemanticError,
        match=r"Redeclare of 'Base\.x' changes causality prefix from 'input' to 'output'",
    ):
        _flatten_inline(
            """
    block Base
        replaceable input Real x;
    end Base;
    block Derived
        extends Base(redeclare output Real x);
    end Derived;""",
            "Derived",
        )


# --- Class-kind compatibility tests (MLS 6.4 / 7.3.2) ---


def test_redeclare_class_same_kind_accepted():
    """Redeclare a replaceable class with another class of the same kind."""
    flat = _flatten_inline(
        """
    model A Real x = 1.0; end A;
    model B extends A; Real y = 2.0; end B;
    model M
        replaceable model Inner = A;
        Inner m;
    end M;
    model D
        extends M(redeclare model Inner = B);
    end D;""",
        "D",
    )
    assert "m.x" in flat.symbols
    assert "m.y" in flat.symbols


def test_redeclare_class_kind_mismatch_rejected():
    """Redeclare that changes class kind from model to connector raises."""
    with pytest.raises(
        tree.ModelicaSemanticError,
        match=r"changes class kind from 'model' to 'connector'",
    ):
        _flatten_inline(
            """
    connector C Real x; end C;
    model M
        replaceable model Inner end Inner;
        Inner m;
    end M;
    model D
        extends M(redeclare connector Inner = C);
    end D;""",
            "D",
        )


def test_redeclare_package_with_package_accepted():
    """Replacing one package with another of the same kind is accepted."""
    flat = _flatten_inline(
        """
    package PA constant Real k = 1.0; end PA;
    package PB constant Real k = 2.0; end PB;
    model M
        replaceable package P = PA;
        parameter Real v = P.k;
    end M;
    model D
        extends M(redeclare package P = PB);
    end D;""",
        "D",
    )
    assert "v" in flat.symbols
    # P.k resolves to PB.k = 2.0 after the redeclare; constant is now folded to literal.
    assert flat.symbols["v"].value == 2.0


def test_redeclare_component_preserves_original_modifier():
    """MLS §7.3.2 spec example: redeclare B a(y=2) merges with original a(x=1).

    Equivalent result: B a(x=1, y=2) — original modifier x=1 must survive.
    """
    flat = _flatten_inline(
        """
    class A
        parameter Real x = 0.0;
    end A;
    class B
        parameter Real x = 0.0;
        parameter Real y = 0.0;
    end B;
    class C
        replaceable A a(x = 1.0);
    end C;
    class D
        extends C(redeclare B a(y = 2.0));
    end D;""",
        "D",
    )
    assert "a.x" in flat.symbols
    assert "a.y" in flat.symbols
    assert flat.symbols["a.x"].value == 1.0
    assert flat.symbols["a.y"].value == 2.0


def test_redeclare_inherited_modifier_preserved():
    """Modifier on replaceable symbol survives redeclare when the modified parameter
    lives only via extends in the redeclare target class (not as a direct symbol).
    """
    instance = parse_and_instantiate_model("RedeclareInheritsMods.mo", "P.User")
    flat = tree.flatten_instance(instance)

    assert "src.offset" in flat.symbols
    assert flat.symbols["src.offset"].value == 1.0


def test_redeclare_qualified_type_name():
    """Redeclare with a qualified (dotted) type name resolves the full path.

    When the redeclare type is a composite name like Inner.Concrete, lookup
    must traverse the full ComponentRef, not just the first segment.
    """
    instance = parse_and_instantiate_model("RedeclareQualifiedType.mo", "P.User")
    flat = tree.flatten_instance(instance)

    assert "src.offset" in flat.symbols
    assert flat.symbols["src.offset"].value == 1.0


if __name__ == "__main__":
    import pytest as _pytest

    _pytest.main([__file__])
