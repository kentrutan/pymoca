#!/usr/bin/env bash
set -eu
if [ "$#" -eq 1 ]; then
    config=$1
else
    echo "usage: $0 CONFIG"
    echo "         possible configs (basic, notebook, develop, all)"
    exit 1
fi
basic="antlr4-python3-runtime==4.7 casadi sympy scipy jinja2 lxml matplotlib"
develop="coverage pytest pytest-cov pytest-xdist pylint flake8 mypy tox"
notebook="jupyterlab pydotplus control slycot"
case "$config" in
    basic)      packages="$basic";;
    develop)    packages="$basic $develop";;
    notebook)   packages="$basic $notebook";;
    all)        packages="$basic $notebook $develop";;
    *) echo "invalid config $config" >&2
       exit 1;;
esac
set -x
conda update -q conda
# We're going to use pip for everything other than conda and python
# so no real need for conda-forge here, but could be added if needed later
# conda config --append channels conda-forge
# Useful for debugging any issues with conda
conda info -a
conda create -n pymoca python=3.9 || echo "environment already created"
source activate pymoca
python -m pip install $packages
