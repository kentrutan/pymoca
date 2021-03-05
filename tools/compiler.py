#!/usr/bin/env python
"""Modelica translator/compiler tool using pymoca
"""
import sys
from typing import List, Union, Tuple
import pathlib
import argparse
import time
import logging

import pymoca.ast
import pymoca.parser
import pymoca.tree
import pymoca.backends.casadi.generator as casadi_gen
import pymoca.backends.sympy.generator as sympy_gen

log = logging.getLogger("pymoca")
logging.basicConfig(stream=sys.stdout)


def parse_file(path: pathlib.Path) -> pymoca.ast:
    """Parse a Modelica file and return AST or None on failure"""
    try:
        log.info('Parsing %s ...', path)
        with path.open() as file:
            ast = pymoca.parser.parse(file.read())
        log.debug(ast)
    except Exception:
        log.exception('Parse error:')
        return None
    return ast


def parse_all(paths: List[pathlib.Path], ast: pymoca.ast) -> List[str]:
    """Parse a list of files and directory trees and add to given AST 
    Return list of files with parse errors
    """
    # Make flat list of .mo files
    files = []
    for path in paths:
        if path.is_file():
            files.append(path)
        elif path.is_dir():
            for glob_path in path.glob('**/*.mo'):
                files.append(glob_path)

    # Parse all .mo files
    error_files = []
    for path in files:
        file_ast = parse_file(path)
        if file_ast:
            ast.extend(file_ast)
        else:
            error_files.append(path)
    return error_files


def flatten_class(class_: str, ast: pymoca.ast) -> pymoca.ast:
    """Flatten given class and return AST

    :param class_: Class to flatten, e.g. 'Package1.Package2.Model'
    :param ast: Previously parsed AST containing the above class
    :return: The flattened AST for the class
    """
    pass
    

def generate(library_ast: pymoca.ast, models_to_generate: List[str], 
             outdir: Union[str, pathlib.Path]) -> List[pathlib.Path]:
    """Given parsed Modelica AST, generate code into outdir for models_to_generate

    Return list of files generated
    """ 
    # profile = cProfile.Profile()
    # profile.enable()
    results = []
    for model in models_to_generate:
        log.info('Generating model for %s ...', model)
        result = casadi_gen.generate(library_ast, model)
        results.append(result)
    # profile.disable()
    # profile.dump_stats(os.path.join(MODEL_DIR, '.'.join([package,model,'profile'])))
    
    return results


def main(args: List[str]) -> int:
    """Parse command line options and do the work"""
    argp = argparse.ArgumentParser(description='Translate Modelica files')
    argp.add_argument('FILE', type=pathlib.Path, nargs='*',
                        help='Modelica file')
    argp.add_argument('-d', '--directory', type=pathlib.Path, action='append',
                        help='recursively read all Modelica files in given directory')
    argp.add_argument('-m', '--model', action='append', 
                        help='model to translate (e.g. Package1.Package2.ModelName)')
    argp.add_argument('-f', '--flatten', action='store_true', help='flatten only')
    argp.add_argument('-p', '--print', action='store_true', help='print parse tree')
    argp.add_argument('-v', '--verbose', action='count', default=0,
                        help='print extra info, more than one gets more verbose')
    genargs = argp.add_argument_group('translation arguments', 
                        'without these, just parse or flatten only')
    genargs.add_argument('-o', '--outdir', type=pathlib.Path, default=pathlib.Path('.'), 
                        help='directory to contain generated model')
    genargs.add_argument('-c', '--casadi', action='store_true', help='generate CasADi')
    genargs.add_argument('-s', '--sympy', action='store_true', help='generate SymPy')
    args = argp.parse_args(args)

    if args.verbose == 0:
        log.setLevel(logging.WARNING)
    elif args.verbose == 1:
        log.setLevel(logging.INFO)
    else:
        log.setLevel(logging.DEBUG)

    if not (args.FILE or args.directory):
        argp.error('At least one of FILE or --directory required')

    # Check that paths exist
    paths = [pathlib.Path(args.outdir)]
    if args.FILE:
        paths += args.FILE
    if args.directory:
        paths += args.directory
    for path in paths:
        stop = False
        if not path.exists():
            log.error('Does not exist: %s', path)
            stop = True
        if stop:
            return 1

    # Collect all Modelica files in given directory
    dir_files = []
    if args.directory:
        for directory in args.directory:
            dir_files += directory.glob('*.mo')

    all_files = args.FILE + dir_files

    tic = time.perf_counter()
    library_ast = pymoca.ast.Tree(name='ModelicaTree')
    errors = parse_all(all_files, library_ast)
    if args.model and not errors:
        try:
            files = generate(library_ast, args.model, args.outdir)
        except Exception as err:
            log.error(err)
    toc = time.perf_counter()
    goodbye_message = f'Finished in {toc - tic:0.4f} seconds'
    if errors:
        goodbye_message = ' '.join([goodbye_message, 
                                    'with {} of {} files with parse errors.'.format(
                                    len(errors), len(all_files))])
    log.info(goodbye_message)
    return len(errors)


if __name__ == '__main__':
    errors = main(sys.argv[1:])
    logging.shutdown()
    sys.exit(errors)
