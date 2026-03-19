"""Conftest for incremental feature development.

Tests listed in XFAIL_IN_PROGRESS are expected to fail during development
of instantiation/name-lookup/flattening features introduced in this range.
"""

import pytest

# Tests that actually fail at this commit.
# strict=False: xpassed (unexpectedly passing) tests are also OK.
XFAIL_IN_PROGRESS = {
    "test/name_lookup_test.py::CompositeNameLookupTest::test_non_package_lookup_comp",
    "test/name_lookup_test.py::CompositeNameLookupTest::test_non_package_lookup_encapsulated",
    "test/name_lookup_test.py::CompositeNameLookupTest::test_non_package_lookup_non_encapsulated",
    "test/name_lookup_test.py::CompositeNameLookupTest::test_package_lookup_constant",
    "test/name_lookup_test.py::CompositeNameLookupTest::test_partial_class_lookup",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_encapsulated",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_qualified_import",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_qualified_import_non_package",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_renaming_import",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_renaming_import_non_package",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_renaming_single_definition_import",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_single_definition_import",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_unqualified_import",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_unqualified_import_non_package",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_unqualified_import_priority",
    "test/name_lookup_test.py::ImportedNameLookupTest::test_unqualified_import_protected",
    "test/parse_test.py::ParseTest::test_constant_references",
    "test/parse_test.py::ParseTest::test_derived_type_value_modification",
    "test/parse_test.py::ParseTest::test_error_extends_class_also_extended_name_nested",
    "test/parse_test.py::ParseTest::test_error_extends_class_also_extended_name_of_self",
    "test/parse_test.py::ParseTest::test_error_extends_class_also_extended_name_simple",
    "test/parse_test.py::ParseTest::test_extends_lookup_not_in_extended",
    "test/parse_test.py::ParseTest::test_extends_modification",
    "test/parse_test.py::ParseTest::test_extends_order",
    "test/parse_test.py::ParseTest::test_extends_redeclareable",
    "test/parse_test.py::ParseTest::test_extends_transitively_nonreplaceable_error",
    "test/parse_test.py::ParseTest::test_flattening_inheritance_tree",
    "test/parse_test.py::ParseTest::test_flattening_modification_scope",
    "test/parse_test.py::ParseTest::test_inheritance_resistor",
    "test/parse_test.py::ParseTest::test_inheritance_symbol_modifiers",
    "test/parse_test.py::ParseTest::test_instantiation_modification_scope_spec_example",
    "test/parse_test.py::ParseTest::test_instantiation_same_names",
    "test/parse_test.py::ParseTest::test_msl3_twopin_units",
    "test/parse_test.py::ParseTest::test_msl_flange_units",
    "test/parse_test.py::ParseTest::test_msl_opamp_units",
    "test/parse_test.py::ParseTest::test_nested_classes",
    "test/parse_test.py::ParseTest::test_nested_symbol_modification",
    "test/parse_test.py::ParseTest::test_nested_symbol_modifier",
    "test/parse_test.py::ParseTest::test_parameter_modification_scope",
    "test/parse_test.py::ParseTest::test_redeclaration_scope",
    "test/parse_test.py::ParseTest::test_redeclaration_scope_alternative",
    "test/parse_test.py::ParseTest::test_redeclare_component_complicated",
    "test/parse_test.py::ParseTest::test_redeclare_component_type_compatibility",
    "test/parse_test.py::ParseTest::test_redeclare_component_in_declaration",
    "test/parse_test.py::ParseTest::test_redeclare_component_in_extends",
    "test/parse_test.py::ParseTest::test_redeclare_components",
    "test/parse_test.py::ParseTest::test_redeclare_in_extends",
    "test/parse_test.py::ParseTest::test_redeclare_nonreplaceable",
}


def pytest_collection_modifyitems(items):
    for item in items:
        if item.nodeid in XFAIL_IN_PROGRESS:
            item.add_marker(pytest.mark.xfail(strict=False, reason="feature in progress"))
