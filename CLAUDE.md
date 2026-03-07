# CLAUDE.md

Pymoca: Modelica-to-CAS translator (Python). Parses `.mo` → AST → flattens class hierarchies → generates CasADi/SymPy/XML output.

## Commands

```bash
pip install -e ".[all]"           # dev install
pytest test                       # all tests
pytest test/parse_test.py -k X    # single test
tox -e py                         # via tox
tox -e coverage                   # with coverage
pre-commit run --all-files        # lint (black + flake8)
```

## Code Style

- Python 3.9+, black (100 char), flake8 (bugbear, comprehensions, import-order)
- Generated code in `src/pymoca/generated/` and `test/generated/` — do not edit

## Git Style

- Atomic commits, title like "ast: Add global name syntax" 50 chars max goal
- Prefixes: `ast:`, `parser:`, `tree:`, combinations (e.g. `ast/parser:`), or `squash:`, `wip:`

## References

- `doc/CLAUDE.md` — MLS spec references and study notes
