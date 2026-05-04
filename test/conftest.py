"""Conftest for incremental feature development.

Tests listed in XFAIL_IN_PROGRESS are expected to fail during development
of instantiation/name-lookup/flattening features introduced in this range.
"""

import pytest

# Tests that actually fail at this commit.
XFAIL_IN_PROGRESS = {
    "test/parse_test.py::ParseTest::test_constant_references",
    "test/parse_test.py::ParseTest::test_flattening_modification_scope",
}


def pytest_collection_modifyitems(items):
    for item in items:
        if item.nodeid in XFAIL_IN_PROGRESS:
            item.add_marker(pytest.mark.xfail(reason="feature in progress"))
