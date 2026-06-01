# Pymoca Development Overview

Pymoca: Modelica-to-CAS translator (Python). Parses `.mo` → AST → flattens class hierarchies → generates CasADi/SymPy/XML output.

## Commands

```bash
pip install -e ".[all]"           # dev install
python antlr/antlr_build.py       # regenerate parser after editing Modelica.g4
pytest test -n $(python3 -c "import os; print(max(1, os.cpu_count() * 3 // 4))")                   # all tests (parallel)
pytest test/parse_test.py -k X -n $(python3 -c "import os; print(max(1, os.cpu_count() * 3 // 4))") # single file (parallel)
pytest test/parse_test.py -k X    # single test (serial, for debugging)
tox -e py                         # via tox
tox -e coverage                   # with coverage
pre-commit run --all-files        # lint (black + flake8)
pytest test -m msl                # run MSL examples pipeline test (skipped by default)
python test/msl_examples_test.py  # MSL pipeline CLI (pass -h for options)
```

## Code Style

- Python 3.9+, black (100 char), flake8 (bugbear, comprehensions, import-order)
- **Comments concise** — Only comment if you can't determine local context from code within surrounding ~80 lines. One line comments is goal, not hard limit. **Don't reference context outside the function, class, module, etc.**
- **Docstrings concise** — {} denotes optional: """One line summary{\n\n¶ to clarify}{\n\nReturns: if not None}{\n\nRaises:}""". **Optional parts only if surrounding context is > ~80 lines.**
- Generated code in `src/pymoca/generated/` and `test/generated/` — do not edit

## Git Style

- **Atomic commits** -- each commit covers one concern, written in present imperative form. One Modelica spec concept = one commit, even if it touches ast.py, parser.py, and tests together, but keep parser and tree changes separate. **Commits with logic changes should include a test.**
- Title: like "tree: Add global name syntax" or "parser: Fix connect equation" — **50 chars max goal**. Describes the *what*, not the *how* (omit implementation details) and **covers the full scope of changes**. Use "Fix" for bug fixes, not the mechanism ("Propagate", "Copy", etc.). Prefixes: `parser:`, `tree:`, `casadi:`, `sympy:` represent pipeline stages, rightmost wins. Only use `test:`, `doc:`, `ci/cd:`, etc, for infrastructure-only changes.
- Body: ¶1 is 1–3 sentences, high-level and durable. Open with an imperative sentence stating what was changed, using spec/domain terms (not code locations or step numbers), with the spec reference at the end if applicable. Follow with *concise* statement of the problem fixed ("Previously these were lost.") or meaning of a new feature. Do not explain the mechanism in ¶1. The mechanism belongs in ¶2.
- ¶2: non-obvious design context summary including mechanism — what ties the changes together, alternatives considered, tradeoffs. Don't reiterate the code diff.
- Additional paragraphs or list: If necessary to describe anything important to understanding the diffs, but not covered above. (If not related to above, should it be a separate commit?). Keep paragraphs to one topic.
- Title and body use the present imperative form, such as "Add," "Fix," or "Remove", especially on initial sentences in paragraphs.
- **No meta-commentary** — don't mention code review, how an issue was found, or process artifacts in commit messages.
- **Linear history** — this project uses a rebase workflow; force-pushing a feature branch after a rebase or commit reword is expected and correct.

## References

- `doc/AGENTS.md` — MLS spec references and study notes
