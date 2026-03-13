"""Pytest configuration for ModelicaCompliance tests."""

import os
import sys

# Add test directory to sys.path so test helper modules (e.g. conftest_parse) can be imported
sys.path.insert(0, os.path.dirname(__file__))


def pytest_configure(config):
    config.addinivalue_line("markers", "compliance: ModelicaCompliance test")
    config.addinivalue_line("markers", "name_lookup: Name lookup level compliance test")
    config.addinivalue_line("markers", "instantiation: Instantiation level compliance test")
    config.addinivalue_line("markers", "flattening: Flattening level compliance test")
    config.addinivalue_line("markers", "value_check: Compile-time value checking test")
