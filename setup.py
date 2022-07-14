#!/usr/bin/env python
"""A python/modelica based simulation environment.

Pymoca contains a Python based compiler for the modelica language
and enables interacting with Modelica easily in Python.

"""

from __future__ import print_function

import os
import fnmatch
import subprocess
import platform
import sys

from distutils.errors import CCompilerError, DistutilsExecError, DistutilsPlatformError
from setuptools import Command, Extension, find_packages, setup

from setuptools.command.build_ext import build_ext

import versioneer

DOCLINES = __doc__.split("\n")

CLASSIFIERS = """\
Development Status :: 1 - Planning
Intended Audience :: Science/Research
Intended Audience :: Developers
License :: OSI Approved :: BSD License
Programming Language :: Python
Programming Language :: Python :: 3
Programming Language :: Other
Topic :: Software Development
Topic :: Scientific/Engineering :: Artificial Intelligence
Topic :: Scientific/Engineering :: Mathematics
Topic :: Scientific/Engineering :: Physics
Topic :: Scientific/Engineering :: Visualization
Operating System :: Microsoft :: Windows
Operating System :: POSIX
Operating System :: Unix
Operating System :: MacOS
Topic :: Software Development :: Code Generators
Topic :: Software Development :: Compilers
Topic :: Software Development :: Embedded Systems
"""


ROOT_DIR = os.path.dirname(os.path.realpath(__file__))

# pylint: disable=no-init, too-few-public-methods


PYTHON_VERSION = '.'.join([str(i) for i in sys.version_info[:3]])
PYTHON_VERSION_REQUIRED = '3.5.0'
if PYTHON_VERSION < PYTHON_VERSION_REQUIRED:
    sys.exit("Sorry, only Python >= {:s} is supported".format(
        PYTHON_VERSION_REQUIRED))


class AntlrBuildCommand(Command):
    """Customized setuptools build command."""

    user_options = [] # type: list

    def initialize_options(self):
        """initialize options"""
        pass

    def finalize_options(self):
        """finalize options"""
        pass

    def run(self):
        "Run the build command"
        call_antlr4('Modelica.g4')


def call_antlr4(arg):
    "calls antlr4 on grammar file"
    # pylint: disable=unused-argument, unused-variable
    antlr_path = os.path.join(ROOT_DIR, "java", "antlr-4.9.3-complete.jar")
    classpath = os.pathsep.join([".", "{:s}".format(antlr_path), "$CLASSPATH"])
    generated = os.path.join(ROOT_DIR, 'src', 'pymoca', 'generated')
    generated_cpp = os.path.join(generated, 'cpp_src')
    cmd = "java -Xmx500M -cp \"{classpath:s}\" org.antlr.v4.Tool {arg:s}" \
          " -o {generated:s} -visitor -listener -Dlanguage=Python3".format(**locals())
    print(cmd)
    proc = subprocess.Popen(cmd.split(), cwd=os.path.join(ROOT_DIR, 'src', 'pymoca'))
    proc.communicate()
    # Generate and patch C++ using speedy-antlr-tool
    cmd = "java -Xmx500M -cp \"{classpath:s}\" org.antlr.v4.Tool {arg:s}" \
          " -o {generated_cpp:s} -visitor -no-listener -Dlanguage=Cpp".format(**locals())
    print(cmd)
    proc = subprocess.Popen(cmd.split(), cwd=os.path.join(ROOT_DIR, 'src', 'pymoca'))
    proc.communicate()
    print('Running speedy-antlr-tool...')
    from speedy_antlr_tool import generate
    generate(
        py_parser_path=os.path.join(generated, "ModelicaParser.py"),
        cpp_output_dir=os.path.join(generated, "cpp_src"),
        entry_rule_names=["stored_definition"],
    )

    with open(os.path.join(generated, '__init__.py'), 'w') as fid:
        fid.write('')


def get_files(path, pattern):
    """
    Recursive file search that is compatible with python3.4 and older
    """
    matches = []
    for root, _, filenames in os.walk(path):
        for filename in fnmatch.filter(filenames, pattern):
            matches.append(os.path.join(root, filename))
    return matches


class BuildFailed(Exception):
    pass


class ve_build_ext(build_ext):
    """
    This class extends setuptools to fail with a common BuildFailed exception
    if a build fails
    """
    def run(self):
        try:
            self.debug = 1
            build_ext.run(self)
        except DistutilsPlatformError:
            raise BuildFailed()

    def build_extension(self, ext):
        try:
            self.debug = 1
            build_ext.build_extension(self, ext)
        except (CCompilerError, DistutilsExecError, DistutilsPlatformError):
            raise BuildFailed()
        except ValueError:
            # this can happen on Windows 64 bit, see Python issue 7511
            if "'path'" in str(sys.exc_info()[1]):  # works with Python 2 and 3
                raise BuildFailed()
            raise


def setup_package(with_binary):
    """
    Setup the package.
    """

    if with_binary:

        target = platform.system().lower()
        platforms = {'windows', 'linux', 'darwin', 'cygwin'}
        for known in platforms:
            if target.startswith(known):
                target = known

        extra_compile_args = {
            'windows': ['/DANTLR4CPP_STATIC', '/Zc:__cplusplus'],
            'linux': ['-std=c++11'],
            'darwin': ['-std=c++11'],
            #'darwin': ['-std=c++11', "-g3", "-O0", "-DDEBUG=0", "-UNDEBUG"],
            'cygwin': ['-std=c++11'],
        }

        # Define an Extension object that describes the Antlr accelerator
        parser_ext = Extension(
            # Extension name shall be at the same level as the sa_mygrammar_parser.py module
            name='pymoca.generated.sa_modelica_cpp_parser',

            # Add the Antlr runtime source directory to the include search path
            include_dirs=["src/pymoca/generated/cpp_src/antlr4-cpp-runtime"],

            # Rather than listing each C++ file (Antlr has a lot!), discover them automatically
            sources=get_files("src/pymoca/generated/cpp_src", "*.cpp"),
            depends=get_files("src/pymoca/generated/cpp_src", "*.h"),

            extra_compile_args=extra_compile_args.get(target, [])
        )
        ext_modules = [parser_ext]
    else:
        ext_modules = []

    extras_require = {
        # Backends
        'casadi': ['casadi >= 3.4.0'],
        'lxml': [
            'lxml >= 3.5.0',
            'scipy >= 0.13.3',
        ],
        'sympy': [
            'sympy >= 0.7.6.1',
            'scipy >= 0.13.3',
        ],
        # Examples
        'examples': [
            'jupyterlab',
            'matplotlib'
        ],
        'all': []  # Automatically generated below

    }
    extras_require['all'] = sorted({r for lst in extras_require.values() for r in lst})

    cmdclass_ = {'antlr': AntlrBuildCommand}
    cmdclass_.update(versioneer.get_cmdclass())
    cmdclass_.update({'build_ext': ve_build_ext})

    setup(
        version=versioneer.get_version(),
        name='pymoca',
        maintainer="James Goppert",
        maintainer_email="james.goppert@gmail.com",
        description=DOCLINES[0],
        long_description="\n".join(DOCLINES[2:]),
        url='https://github.com/pymoca/pymoca',
        author='James Goppert',
        author_email='james.goppert@gmail.com',
        download_url='https://github.com/pymoca/pymoca',
        license='BSD',
        classifiers=[_f for _f in CLASSIFIERS.split('\n') if _f],
        platforms=["Windows", "Linux", "Solaris", "Mac OS-X", "Unix"],
        install_requires=[
            "antlr4-python3-runtime == 4.9.*",
            "numpy >= 1.8.2",
        ],
        tests_require=['coverage >= 3.7.1', 'pytest', 'pytest-runner'],
        extras_require=extras_require,
        python_requires='>=3.5',
        packages=find_packages("src"),
        package_dir={"": "src"},
        include_package_data=True,
        exclude_package_data={"pymoca.generated": ["cpp_src"]},
        ext_modules=ext_modules,
        cmdclass=cmdclass_
    )


if __name__ == '__main__':
    # Detect if an alternate interpreter is being used
    is_jython = "java" in sys.platform
    is_pypy = hasattr(sys, "pypy_version_info")

    # Force using fallback if using an alternate interpreter
    using_fallback = is_jython or is_pypy

    if not using_fallback:
        try:
            setup_package(with_binary=True)
        except BuildFailed:
            if 'SPAM_EXAMPLE_REQUIRE_CI_BINARY_BUILD' in os.environ:
                # Require build to pass if running in travis-ci
                raise
            else:
                using_fallback = True

    if using_fallback:
        setup_package(with_binary=False)
