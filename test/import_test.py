#!/usr/bin/env python
"""
Tests for Modelica import semantics.
"""

from conftest_parse import (
    MSL3_DIR,
    MSL4_DIR,
    check_instance_tree_is_all_instances,
    parse_dir_files,
    parse_imports_file,
    parse_model_files,
)

from pymoca import ast
from pymoca import tree

import pytest


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


if __name__ == "__main__":
    import pytest as _pytest

    _pytest.main([__file__])
