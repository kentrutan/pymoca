"""Pytest configuration for ModelicaCompliance and MSL tests."""

import os
import sys

# Ratio of the total os.cpu_count() to use for testing
XDIST_CPU_RATIO = 3 / 4

# Add test directory to sys.path so test helper modules (e.g. conftest_parse) can be imported
sys.path.insert(0, os.path.dirname(__file__))


def pytest_configure(config):
    config.addinivalue_line("markers", "compliance: ModelicaCompliance test")
    config.addinivalue_line("markers", "flattening: Flattening level compliance test")
    config.addinivalue_line("markers", "msl: MSL examples pipeline test")


def pytest_xdist_auto_num_workers(config):
    """Leave some cpu power open for other work during tests using `-n auto`"""
    if config.option.numprocesses != "auto":
        return None
    cpu_count = os.cpu_count()
    return int(cpu_count * XDIST_CPU_RATIO) if cpu_count else None
