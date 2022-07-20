"""Test speedy-antlr C++ parse results are equivalent to pure Python results"""
import os.path
import unittest
import sys
sys.path.insert(0, '/Users/rutanwk/modelica/antlr/speedy-antlr-tool-wkr')

import antlr4
from speedy_antlr_tool.validate import validate_top_ctx

from pymoca.generated import sa_modelica

MY_DIR = os.path.dirname(os.path.realpath(__file__))
MODEL_DIR = os.path.join(MY_DIR, 'models')

@unittest.skip('WIP: TestEquivalent')
class TestEquivalent(unittest.TestCase):
    """This is based on speedy_antlr_example test"""

    def test_aircraft(self):
        """Test for basic Modelica"""
        self._compare_py_and_cpp('Aircraft.mo')

    def test_ifelse(self):
        """Test list labels in grammar"""
        self._compare_py_and_cpp('IfElse.mo')

    def _compare_py_and_cpp(self, filename):
        """
        Test if C++ and pure python implementations result in an equivalent
        parse tree
        """

        # Test requires the C++ implementation to exist
        self.assertTrue(sa_modelica.USE_CPP_IMPLEMENTATION)

        with open(os.path.join(MODEL_DIR, filename), 'r', encoding='utf-8') as f:
            txt = f.read()

        stream = antlr4.InputStream(txt)

        # Parse the same stream using both parsers
        py = sa_modelica._py_parse(stream, "stored_definition")
        cpp = sa_modelica._cpp_parse(stream, "stored_definition")

        # Validate them!
        validate_top_ctx(py, cpp)


if __name__ == '__main__':
    unittest.main(defaultTest='TestEquivalent.test_ifelse')

