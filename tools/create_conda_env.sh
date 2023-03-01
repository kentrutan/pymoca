#!/usr/bin/env bash
set -eu
env_name="pymoca"
case "$#" in
    1)  config=$1;;
    2)  config=$1; env_name=$2;;
    *)  echo "usage: $0 CONFIG [ENV_NAME]"
        echo "         CONFIG is one of (basic, notebook, develop, all)"
        echo "         ENV_NAME is environment name (default pymoca)"
        exit 1;;
esac
basic="antlr4-python3-runtime==4.9.* casadi sympy scipy jinja2 lxml matplotlib"
develop="speedy-antlr-tool==1.3.* coverage pytest pytest-cov pytest-xdist pylint flake8 mypy tox versioneer"
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
conda create -n $env_name python=3.9 || echo "environment already created"
source activate $env_name
python -m pip install $packages
