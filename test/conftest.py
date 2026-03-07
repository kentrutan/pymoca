"""Pytest configuration for ModelicaCompliance tests."""


def pytest_configure(config):
    config.addinivalue_line("markers", "compliance: ModelicaCompliance test")
    config.addinivalue_line("markers", "name_lookup: Name lookup level compliance test")
    config.addinivalue_line("markers", "instantiation: Instantiation level compliance test")
    config.addinivalue_line("markers", "flattening: Flattening level compliance test")
