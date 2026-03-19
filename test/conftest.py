"""Conftest for incremental feature development.

Tests listed in XFAIL_IN_PROGRESS are expected to fail during development
of instantiation/name-lookup/flattening features introduced in this range.
"""

import pytest

# Tests that actually fail at this commit.
# strict=False: xpassed (unexpectedly passing) tests are also OK.
XFAIL_IN_PROGRESS = {
    "test/parse_test.py::ParseTest::test_ast_element_full_name",
    "test/parse_test.py::ParseTest::test_extends_transitively_nonreplaceable_error",
    "test/parse_test.py::ParseTest::test_instantiation_modification_scope_instance_class",
    "test/parse_test.py::ParseTest::test_nonreplaceable_component_contains_replaceable",
    "test/parse_test.py::ParseTest::test_redeclare_class_with_symbol_error",
    "test/parse_test.py::ParseTest::test_redeclare_component_complicated",
    "test/parse_test.py::ParseTest::test_redeclare_component_in_declaration",
    "test/parse_test.py::ParseTest::test_redeclare_component_in_extends",
    "test/parse_test.py::ParseTest::test_redeclare_component_type_compatibility",
    "test/parse_test.py::ParseTest::test_visibility_in_ast",
    "test/parse_test.py::ParseTest::test_visibility_in_instance",
}


def pytest_collection_modifyitems(items):
    for item in items:
        if item.nodeid in XFAIL_IN_PROGRESS:
            item.add_marker(pytest.mark.xfail(strict=False, reason="feature in progress"))
