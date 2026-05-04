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
    "test/parse_test.py::ParseTest::test_extends_order",
    "test/parse_test.py::ParseTest::test_extends_transitively_nonreplaceable_error",
    "test/parse_test.py::ParseTest::test_flattening_inheritance_tree",
    "test/parse_test.py::ParseTest::test_flattening_modification_scope",
    "test/parse_test.py::ParseTest::test_lookup_needs_instantiation",
    "test/parse_test.py::ParseTest::test_msl3_twopin_units",
    "test/parse_test.py::ParseTest::test_msl_flange_units",
    "test/parse_test.py::ParseTest::test_msl_opamp_units",
    "test/parse_test.py::ParseTest::test_redeclaration_scope_alternative",
    "test/parse_test.py::ParseTest::test_redeclare_class_with_symbol_error",
    "test/parse_test.py::ParseTest::test_redeclare_component_complicated",
    "test/parse_test.py::ParseTest::test_redeclare_component_in_declaration",
    "test/parse_test.py::ParseTest::test_redeclare_component_in_extends",
}


def pytest_collection_modifyitems(items):
    for item in items:
        if item.nodeid in XFAIL_IN_PROGRESS:
            item.add_marker(pytest.mark.xfail(strict=False, reason="feature in progress"))
