#!/usr/bin/env python
"""Modelica translator/compiler tool using pymoca

This duplicates some things in casadi.api, but is useful for other backends
TODO: Perhaps refactor the parts common with casadi backend
"""
from __future__ import generators
import sys
import os
from typing import List, Tuple, Union
from pathlib import Path
import argparse
import time
import logging
import json

from . import __version__, ast, tree, parser

from .backends.sympy import generator as sympy_gen
from .backends.casadi import api as casadi_api

log = logging.getLogger("pymoca")
logging.basicConfig(stream=sys.stderr)

# In lieu of a pluggable backend api, we'll use this for now.
TRANSLATOR_TYPES = ('casadi', 'sympy')


def list_modelica_files(paths: List[Path]) -> List[Path]:
    """Find all Modelica files in given paths (can be files and directories)

    :param paths: List of Paths to search
    :return: List of valid Modelica file Paths found
    """
    # Make flat list of .mo files
    files = []
    for path in paths:
        if path.is_file() and path.suffix == '.mo':
            files.append(path)
        elif path.is_dir():
            for glob_path in path.glob('**/*.mo'):
                files.append(glob_path)
    return files


def parse_file(path: Path) -> Union[ast.Tree, None]:
    """Parse a Modelica file and return AST or None on failure

    :param path: single Path containing Modelica code
    :return: parsed ast.Tree or None on error
    """
    ast = None
    try:
        log.info('Parsing %s ...', path)
        with path.open(encoding='utf-8') as file:
            ast = parser.parse(file.read())
        if ast is None:
            log.error('Syntax error in file "%s"', path)
        elif log.level == logging.DEBUG:
            log.debug(json.dumps(ast.to_json(ast), indent=2))
    # KeyError and AttributeError are problems in ASTListener
    except (KeyError, AttributeError, IOError, OSError):
        if log.level in (logging.DEBUG, logging.INFO):
            log.exception('Parse error in file "%s"', path)
        else:
            log.error('Parse error in file "%s"', path)
        return None
    return ast


def parse_all(paths: List[Path], ast: ast.Tree = None) -> Tuple[List[Path], List[Path]]:
    """Parse a list of files and directory trees and add to given AST

    :param paths: List of files and diretory trees to parse
    :param ast: Optional ast.Tree to add parsed AST to
    :return: tuple (list of all .mo files, list of files with parse errors)
    """
    files = list_modelica_files(paths)
    if not files:
        return [], []
    if not ast:
        ast = ast.Tree(name='ModelicaTree')
    # Parse all .mo files
    error_files = []
    for path in files:
        file_ast = parse_file(path)
        if file_ast:
            ast.extend(file_ast)
        else:
            error_files.append(path)
    return files, error_files


def flatten_class(library_ast: ast.Tree, class_: str) -> ast.Tree:
    """Flatten given class and return AST

    :param library_ast: Previously parsed AST containing the above class
    :param class_: Class to flatten, e.g. 'Package1.Package2.Model'
    :return: flattened ast.Tree
    """
    log.info('Flattening %s ...', class_)
    component_ref = ast.ComponentRef.from_string(class_)
    flat_tree = tree.flatten(library_ast, component_ref)
    if log.level == logging.DEBUG:
        log.debug(json.dumps(flat_tree.to_json(flat_tree), indent=2))
    return flat_tree


def translate(library_ast: ast.Tree, model: str,
              translator: str, options: dict, outdir: Path = Path('.')) -> bool:
    """Given parsed Modelica AST, generate code for model into given directory

    :param library_ast: Previously parsed AST containing the above model class
    :param model: Modelica Class to generate code for
    :param translator: translator to use (e.g. 'sympy' or 'casadi')
    :param options: dict of options to pass to translator
    :param outdir: directory to put results in
    :return: True on success, False on failure
    """
    log.info('Generating model for %s ...', model)
    # Currenly only support sympy; envision others being added in future
    if translator == 'sympy':
        try:
            result = sympy_gen.generate(library_ast, model, options)
            outfile = outdir.joinpath(model + '.py')
            with outfile.open('w') as file:
                file.write(result)
        except (IOError, OSError):
            if log.level is logging.DEBUG:
                log.exception('Error writing "%s"', outfile)
            else:
                log.error('Error writing "%s"', outfile)
            return False
        except KeyError:
            log.exception('Problem translating %s to SymPy', model)
            return False
    else:
        raise NotImplementedError('Translator for {} not implemented'.format(translator))
    return True

class MyArgumentParser(argparse.ArgumentParser):
    """Make options file treat each space separated word as a separate argument"""
    def convert_arg_line_to_args(self, arg_line):
        return arg_line.split()

def cli() -> int:
    '''Parse command line options and do the work

    Command line arguments are taken from sys.argv("-h" gives help printout).
    :return: number of usage errors (not parse errors)
    '''
    example_help = '''
    Examples:

        Parse all files in "MSL-4.0.x" directory tree, printing any errors and time taken:
            python tools/compiler.py -v test/libraries/MSL-4.0.x

        Test flattening OpAmp model in "MSL-4.0.x/Modelica" library without generating translated code:
            python tools/compiler.py -v -p test/libraries/MSL-4.0.x -m Modelica.Electrical.Analog.Basic.OpAmp

        Generate SymPy code for "Spring" model, putting it in the "sympy_models" directory:
            python tools/compiler.py -v -p test/models -m Spring -t sympy -o sympy_models

        Read some options above from a file:
            python tools/compiler.py -v -p test/models -m Spring @args.txt

            where the args.txt file contains space-separated arguments:
            -t sympy
            -o sympy_models
    '''
    argp = MyArgumentParser(description='Translate Modelica code to specified output code',
                            epilog=example_help,
                            formatter_class=argparse.RawDescriptionHelpFormatter,
                            fromfile_prefix_chars='@')
    argp.add_argument('PATHNAME', type=Path, nargs='*',
                    help='Modelica files and directory trees, all of which are parsed')
    argp.add_argument('-p', '--path', action='append', default=list(),
                    help='"{}" separated path list to add to MODELICAPATH'.format(os.pathsep))
    argp.add_argument('-v', '--verbose', action='count', default=0,
                    help='print extra info; -vv is even more verbose')
    argp.add_argument('--version', action='version', version=__version__,
                    help='print pymoca version')
    genargs = argp.add_argument_group('translation arguments',
                    'without these, just parse only')
    genargs.add_argument('-m', '--model', action='append',
                    help='model to translate (e.g. Package1.Package2.ModelName); '
                    'if not specified, then flatten only')
    genargs.add_argument('-t', '--translator', choices=TRANSLATOR_TYPES,
                    help='translate Modelica to this output type')
    genargs.add_argument('-O', '--option', action='append',
                    help='translator option in the form NAME=VALUE with no spaces or quoted')
    genargs.add_argument('-o', '--outdir', type=Path, default='.',
                    help='directory to contain output code')

    args = argp.parse_args()
    errors = 0  # For additional argument checks beyond what argparse does

    if args.verbose == 0:
        log.setLevel(logging.WARNING)
    elif args.verbose == 1:
        log.setLevel(logging.INFO)
    else:
        log.setLevel(logging.DEBUG)

    # Check for invalid option combinations (argp.error will exit)
    modelicapath_env = os.getenv('MODELICAPATH', default='')
    if not (args.PATHNAME or args.path or modelicapath_env):
            argp.error('PATHNAME or -p/--path or environment variable MODELICAPATH required')
    if (args.path or modelicapath_env) and not (args.model or args.PATHNAME):
            argp.error('Nothing to do. Specify at least -m/--model')
    if args.translator and not args.model:
        argp.error('-t/--translator requires -m/--model')
    if not args.translator:
        if args.option:
            log.warning('Ignoring -O (options only used with -t/--translator)')
        if args.model:
            log.info('No translator specified (-t option), flattening model only')
    elif args.translator == 'casadi' and not args.PATHNAME:
        argp.error('--translator casadi requires at least one PATHNAME')

    # Check that paths exist
    if not args.outdir.is_dir():
        if args.outdir.is_file():
            log.error('Invalid output directory: "%s"', args.outdir)
            errors += 1
        else:
            try:
                args.outdir.mkdir()
            except OSError:
                log.error('Unable to create output directory: "%s', args.outdir)
                errors += 1
    for path in args.PATHNAME:
        if not path.exists():
            log.error('File or directory does not exist: "%s"', path)
            errors += 1
    paths = []
    for mparg in args.path + [modelicapath_env]:
        for path in mparg.split(os.pathsep):
            if path:
                paths.append(Path(path))
    modelica_path = []
    for path in paths:
        if not path.is_dir():
            log.error('Invalid MODELICAPATH directory: "%s"', path)
            errors += 1
        else:
            modelica_path.append(path)
    if not modelica_path and (args.path or modelicapath_env):
        # MODELICAPATH options specified but no valid paths found
        log.error('No valid MODELICAPATH directories given')
        errors += 1

    # Build translator options dict from args
    options = dict()
    if args.option:
        for opt in args.option:
            optsplit = opt.split('=')
            if len(optsplit) == 2:
                # Convert True/False values, otherwise string value as-is
                value_lower = optsplit[1].lower()
                if value_lower == 'true':
                    optsplit[1] = True
                elif value_lower == 'false':
                    optsplit[1] = False
                options[optsplit[0]] = optsplit[1]
            else:
                log.error('Invalid option syntax (need -O NAME=VALUE): "%s"', opt)
                errors += 1

    if errors:
        return errors

    # Scan and cache MODELICAPATH directories and files
    modelicapath_list = [] # type: List[parser.ModelicaPathNode]
    for path in modelica_path:
        modelicapath_list.append(parser.ModelicaPathNode(path))

    tic = time.perf_counter()
    error_files = [] # type: List[Path]
    modelica_files = [] # type: List[Path]
    if args.translator == 'sympy' or not args.translator:

        library_ast = parser.Root(name='ModelicaTree', modelicapath=modelicapath_list)
        modelica_files, error_files = parse_all(args.PATHNAME, library_ast)
        if not modelica_files and not modelicapath_list:
            errors += 1
            log.error('No Modelica files in given PATHNAMEs')
        elif error_files:
            errors += len(error_files)
        if not errors and args.model:
            for model in args.model:
                if args.translator:
                    translate(library_ast, model, 'sympy', options, args.outdir)
                elif args.model:
                    try:
                        _ = flatten_class(library_ast, model)
                    # tree.flatten_class can throw Exception in several places
                    except Exception as exception:  # pylint: disable=broad-except
                        if log.level is logging.DEBUG:
                            log.exception('Error flattening %s', model)
                        else:
                            log.error('Error flattening %s: %s', model, exception)
                        errors += 1

    elif args.translator == 'casadi':

        modelica_files = list_modelica_files(args.PATHNAME)
        if not modelica_files:
            errors += 1
            log.error('No Modelica files in given PATHNAMEs')
        else:
            for model in args.model:
                # Infer model directory to pass to casadi.api
                model_dir = None
                for path in modelica_files:
                    if path.stem == model:
                        if model_dir:
                            # More than one found (ambiguous)
                            log.error('More than one Modelica file found for %s', model)
                            errors += 1
                            model_dir =  None
                            break
                        model_dir = path.parent
                if not model_dir:
                    log.error('No unique Modelica file corresponding to model %s', model)
                else:
                    log.info('Generating model for %s ...', model)
                    try:
                        _ = casadi_api.transfer_model(str(model_dir), model, options)
                    # TODO: Figure out more specific Exceptions for CasADi transfer_model
                    except Exception:  # pylint: disable=broad-except
                        if log.level is logging.DEBUG:
                            log.exception('Problem generating CasADi model %s', model)
                        else:
                            log.error('Problem generating CasADi model %s', model)
                        errors += 1

    else:
        # Should never get here because argparse should have caught above
        assert args.translator in TRANSLATOR_TYPES
    toc = time.perf_counter()

    goodbye_message = 'Finished in {:0.4f} seconds'.format(toc - tic)
    if error_files:
        goodbye_message = ' '.join([goodbye_message,
                                    'with {} of {} files with parse errors.'.format(
                                    len(error_files), len(modelica_files))])
    log.info(goodbye_message)
    return errors

def main(argv: List[str]) -> int:
    """Entry point to command line interface from imported module

    :param argv: Command line arguments
    """
    if not isinstance(argv, list) or not all(isinstance(x, str) for x in argv):
        raise ValueError('argv must be list of str')
    sys.argv = argv
    return cli()

if __name__ == '__main__':
    err = cli()
    logging.shutdown()
    sys.exit(err)
