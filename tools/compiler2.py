"""Caterpillar version of compiler.py
"""
import sys
from typing import List, Union
import pathlib
import argparse
import time
import logging

import pymoca.parser
import pymoca.ast
from pymoca.backends.casadi import generator

log = logging.getLogger("moto")
if sys.platform == 'win32':
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

def parse_all(paths: List[pathlib.Path], ast: pymoca.ast) -> (int, int):
    """Parse a list of files and directory trees and add to given AST 
    Return tuple of (number of files, number of errors)
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
    files_count = 0
    errors_count = 0
    for path in files:
        files_count += 1
        file_ast = parse_file(path)
        if file_ast:
            ast.extend(file_ast)
        else:
            errors_count += 1
    return (files_count, errors_count)

def generate(library_ast: pymoca.ast, models_to_generate: List[str], 
             outdir: Union[str, pathlib.Path]) -> List[pathlib.Path]:
    """Given parsed Modelica AST, generate code into outdir for models_to_generate

    Return list of C++ files generated
    """ 
    # profile = cProfile.Profile()
    # profile.enable()
    results = []
    for model in models_to_generate:
        log.info('Generating model for %s ...', model)
        result = generator.generate(library_ast, model, log)
        results.append(result)
    # profile.disable()
    # profile.dump_stats(os.path.join(MODEL_DIR, '.'.join([package,model,'profile'])))
    
    return results

def main(args: List[str]) -> int:
    # TODO: Configure arguments for ease of use (they are evolving with development)
    argp = argparse.ArgumentParser()
    argp.add_argument('PATH', type=pathlib.Path, nargs='+')
    argp.add_argument('-v', '--verbose', help='print input file name and parse tree, etc.', action='count', default=0)
    argp.add_argument('-p', '--parse', help='parse only', action='store_true')
    argp.add_argument('-o', '--outdir', help='directory to contain generated model', type=pathlib.Path, default=pathlib.Path('.'))
    argp.add_argument('-m', '--model', help='Modelica model(s) (e.g. Package1.Package2.ModelName) to generate', nargs='+')
    args = argp.parse_args(args)
    if args.verbose == 0:
        log.setLevel(logging.WARNING)
    elif args.verbose == 1:
        log.setLevel(logging.INFO)
    else:
        log.setLevel(logging.DEBUG)
    # Check that paths exist
    for path in args.PATH + [pathlib.Path(args.outdir)]:
        stop = False
        if not path.exists():
            log.error('Does not exist: %s', path)
            stop = True
        if stop:
            return 1
    tic = time.perf_counter()
    library_ast = pymoca.ast.Tree(name='ModelicaLibrary')
    files, errors = parse_all(args.PATH, library_ast)
    if not args.parse and not errors and args.model:
        try:
            c_files = generate(library_ast, args.model, args.outdir)
        except generator.GeneratorError as err:
            log.error(err)
    toc = time.perf_counter()
    goodbye_message = f'Finished in {toc - tic:0.4f} seconds'
    if errors:
        goodbye_message = ' '.join([goodbye_message, f'with {errors} of {files} files with parse errors.'])
    log.info(goodbye_message)
    return errors

if __name__ == '__main__':
    errors = main(sys.argv[1:])
    logging.shutdown()
    sys.exit(errors)
