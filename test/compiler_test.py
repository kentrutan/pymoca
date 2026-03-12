#!/usr/bin/env python
"""
Test compiler tool

Leave testing of pymoca details to other tests.
"""

import os

import pytest

import tools.compiler

FILE_DIR = os.path.dirname(os.path.realpath(__file__))
MODEL_DIR = os.path.join(FILE_DIR, "models")
GENERATED_DIR = os.path.join(FILE_DIR, "generated")
SPRING_MODEL = os.path.join(MODEL_DIR, "Spring.mo")
AIRCRAFT_MODEL = os.path.join(MODEL_DIR, "Aircraft.mo")
BOUNCING_BALL_XML = os.path.join(MODEL_DIR, "bouncing-ball.xml")


def run_compiler(args, check_errors=True):
    """Run compiler with all arguments given as a string
    When argparse catches errors in command arguments it calls sys.exit(2).
    If arguments pass argparse, number of errors is expected to be returned from compiler.
    """
    exitval = tools.compiler.main(args.split())
    if check_errors:
        assert not exitval, "tools.compiler " + args
    return exitval


def run_compiler_add_model_dir(options, model="Spring", filename="Spring.mo"):
    "Run compiler on filename in tests directory for Modelica class given by model"
    filename_path = os.path.join(MODEL_DIR, filename)
    args = options + " -m" + model + " " + filename_path
    return run_compiler(args)


def test_argparse_checks_good():
    "Stuff that argparse should handle ok and exit"
    arg_examples = [
        "-h",
        "--version",
    ]
    # argparse does a sys.exit(0) with these
    for args in arg_examples:
        with pytest.raises(SystemExit) as exc_info:
            run_compiler(args, check_errors=False)
        assert exc_info.value.code == 0


def test_argparse_checks_bad():
    "Stuff that argparse should catch and exit"
    arg_examples = [
        "",  # Must give at least a PATH
        "-m",
        "-t",
        "-O",
        "-o",  # These expect an argument
        "-t sausage",  # Invalid target
    ]
    # argparse does a sys.exit(2) with these
    for args in arg_examples:
        with pytest.raises(SystemExit) as exc_info:
            run_compiler(args, check_errors=False)
        assert exc_info.value.code == 2


def test_bad_argument_combinations():
    "Bad option combinations caught outside argparse"
    # --target requires --model
    # This one uses argparse.error which does a sys.exit(2)
    with pytest.raises(SystemExit) as exc_info:
        run_compiler("-tsympy " + SPRING_MODEL, check_errors=False)
    assert exc_info.value.code == 2

    # These check multiple errors before exiting
    # List of tuples of (args, number_of_expected_errors)
    bad_options = [
        # 1) Output file doesn't exist
        # 2) Modelica file doesn't exist
        ("-o bacon sausage", 2),
        # 1) No .mo files given (assume none in GENERATED_DIR)
        (GENERATED_DIR, 1),
        # 1) Modelica file does not exist
        ("sausage", 1),
        # 1) Bad target option syntax
        # 2) Give a file instead of a directory for output Path
        # 3) Give a Modelica file that does not exist
        (" ".join(("-t casadi -m Spring -O eggs -o", SPRING_MODEL, "spam")), 3),
    ]
    for args, expected_errors in bad_options:
        errors = run_compiler(args, check_errors=False)
        assert errors == expected_errors


def test_parse_only():
    "If only a Modelica file is given, then compiler tool stops after parse"
    # Parse one file
    run_compiler(SPRING_MODEL)
    # Parse two files
    run_compiler(" ".join([SPRING_MODEL, AIRCRAFT_MODEL]))
    # Parse all files in a directory and a given file
    run_compiler(" ".join([MODEL_DIR, SPRING_MODEL]))


def test_flatten_only():
    "If model is given and target is not, then compiler tool stops after flatten"
    # Parse and flatten
    run_compiler("-v -m Spring " + SPRING_MODEL)
    run_compiler("-v -m Aircraft " + AIRCRAFT_MODEL)
    run_compiler("-v -m Spring -m Aircraft " + " ".join([SPRING_MODEL, AIRCRAFT_MODEL]))


def test_casadi_options_good():
    "CasADi generation expected to run ok"
    # Run examples on default Spring model, give other options here
    arg_examples = [
        "-vv -Ospam=eggs",  # Flatten only, -O ignored
        "-v -tcasadi",  # -v = logging.INFO
        "-vv --target=casadi",  # -vv = logging.DEBUG
        "-t=casadi -Ocache=False -Ocodegen=False -Ocheck_balanced=True",
    ]
    for args in arg_examples:
        run_compiler_add_model_dir(args)


def test_casadi_options_bad():
    "CasADi generation expected to fail"
    # List of tuples of (args, number_of_expected_errors)
    bad_options = [
        # 1) No .mo files given (assume none in GENERATED_DIR)
        ("-tcasadi -mspam " + GENERATED_DIR, 1),
        # 1) Given Modelica file does not exist
        ("-tcasadi -mspam eggs", 1),
        # 1) Can't infer Modelica file from given model (Spring also in Package subdir)
        ("-tcasadi -mSpring " + MODEL_DIR, 1),
    ]
    for args, expected_errors in bad_options:
        errors = run_compiler(args, check_errors=False)
        assert errors == expected_errors


def test_sympy_options_good():
    "SymPy options that should produce good output"
    # Run examples on default Spring model
    arg_examples = [
        "-t sympy -o " + GENERATED_DIR + " -vv",
    ]
    for args in arg_examples:
        run_compiler_add_model_dir(args)


def test_sympy_options_bad():
    "SymPy generation expected to fail"
    # List of tuples of (args, number_of_expected_errors)
    bad_options = [
        # 1) No .mo files found (assume none in GENERATED_DIR)
        ("-tsympy -mspam " + GENERATED_DIR, 1),
        # 1) Given Modelica file does not exist
        ("-tsympy -mspam eggs", 1),
        # 1) Parse error
        ("-tsympy -mSpring " + BOUNCING_BALL_XML, 1),
    ]
    for args, expected_errors in bad_options:
        errors = run_compiler(args, check_errors=False)
        assert errors == expected_errors
