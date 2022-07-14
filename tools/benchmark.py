import sys
import os
import timeit
from pathlib import Path
from typing import Callable

# TODO: Make relative or use relative package imports
sys.path.insert(0, '/Users/rutanwk/modelica/pymoca')
sys.path.insert(0, '/Users/rutanwk/modelica/pymoca/build/lib.macosx-10.9-x86_64-3.9')

from pymoca import parser
from pymoca import tree
from pymoca import ast
from pymoca.generated import sa_modelica
import tools.compiler


MY_DIR = Path(__file__).parent
# MODEL_DIR = os.path.join(MY_DIR, 'models')
# MSL3_DIR = os.path.join(MY_DIR, 'libraries', 'MSL-3.2.3')
MSL4_DIR = MY_DIR / '..' / 'test' / 'libraries' / 'MSL-4.0.x'
MSL4_DIR = MSL4_DIR.resolve()
ELECTRICAL_DIR = MSL4_DIR / 'Modelica' / 'Electrical'
PYMOCA_MODEL_DIR = MY_DIR / '..' / 'test' / 'models'
PYMOCA_MODEL_DIR = PYMOCA_MODEL_DIR.resolve()

def test_flatten_every_MSL_example():
    mp = parser.ModelicaPathNode(MSL4_DIR)
    modelicapath = [mp]
    library_tree = parser.Root(name='ModelicaTree', modelicapath=modelicapath)

    msl_path = Path(MSL4_DIR)
    root_index = len(msl_path.parts)
    for path in msl_path.glob('**/Examples/**/*.mo'):
        if path.name == 'package.mo':
            continue
        parts = path.parts
        model_name = '.'.join(parts[root_index:-1] + (path.stem,))
        flat_class = ast.ComponentRef.from_string(model_name)
        try:
            flat_tree = tree.flatten(library_tree, flat_class)
            print('Flattened {}'.format(flat_tree.classes[0].name))
        except Exception as e:
            print('Error Flattening {}:  {}'.format(model_name, e))
            #raise e


def run_compiler(args, check_errors=True):
    'Run compiler with all arguments given as a string'
    exitval = tools.compiler.main(args.split())
    if check_errors and exitval:
        print('Error running tools.compiler ' + args)
    return exitval

def parse_file(pathname):
    'Parse given full path name and return parsed ast.Tree'
    with open(pathname, 'r') as mo_file:
        txt = mo_file.read()
    return parser.parse(txt)

def parse_dir_files(directory, *pathnames):
    """Parse given file paths relative to dir and return parsed ast.Tree

    Dir is os-specific and paths are unix-style but are transformed to os specific.
    """
    tree = None
    for pathname in pathnames:
        split_path = pathname.split('/')
        full_path = os.path.join(directory, *split_path)
        file_tree = parse_file(full_path)
        if tree:
            tree.extend(file_tree)
        else:
            tree = file_tree
    return tree


def test_parse_all_MSL():
    'If only a Modelica file is given, then compiler tool stops after parse'
    # Parse all files in a directory
    run_compiler(str(MSL4_DIR))

def test_parse_MSL_Electrical():
    # Parse all files in a directory
    run_compiler(str(ELECTRICAL_DIR))

def test_msl_opamp_keep_connectors():
    """Test flattening opamp component, keeping connectors"""
    library_tree = parse_dir_files(str(MSL4_DIR),
        'Modelica/Icons.mo',
        'Modelica/Units.mo',
        'Modelica/Electrical/package.mo',
        'Modelica/Electrical/Analog/Interfaces/PositivePin.mo',
        'Modelica/Electrical/Analog/Interfaces/NegativePin.mo',
        'Modelica/Electrical/Analog/Basic/OpAmp.mo',
    )
    model_name = 'Modelica.Electrical.Analog.Basic.OpAmp'
    flat_class = ast.ComponentRef.from_string(model_name)
    flat_tree = tree.flatten(library_tree, flat_class, component=True)

def test_parse_pymoca_models():
    # Parse all files in a directory
    run_compiler(str(PYMOCA_MODEL_DIR))


def benchmark(function, count):
    sa_modelica.USE_CPP_IMPLEMENTATION = True
    cpp_elapsed = timeit.timeit(function, number=count)
    sa_modelica.USE_CPP_IMPLEMENTATION = False
    py_elapsed = timeit.timeit(function, number=count)

    py_elapsed = py_elapsed / count
    cpp_elapsed = cpp_elapsed / count

    print("py_elapsed:  %.3f" % (py_elapsed), "seconds")
    print("cpp_elapsed: %.3f" % (cpp_elapsed), "seconds")
    print("Speedup: %.2f" % (py_elapsed / cpp_elapsed))

def benchmark_auto(function):
    sa_modelica.USE_CPP_IMPLEMENTATION = True
    cpp_elapsed = timeit.timeit(function)
    sa_modelica.USE_CPP_IMPLEMENTATION = False
    py_elapsed = timeit.timeit(function)

    py_elapsed = py_elapsed
    cpp_elapsed = cpp_elapsed

    print("py_elapsed:  %.3f" % (py_elapsed), "seconds")
    print("cpp_elapsed: %.3f" % (cpp_elapsed), "seconds")
    print("Speedup: %.2f" % (py_elapsed / cpp_elapsed))

if __name__ == '__main__':
    benchmark(test_flatten_every_MSL_example, 5)
    # benchmark(test_parse_all_MSL, 5)
    # benchmark(test_parse_MSL_Electrical, 5)
    # benchmark(test_msl_opamp_keep_connectors, 10)
    # benchmark(test_parse_pymoca_models, 10)
    # benchmark(test_parse_pymoca_models, 10)
