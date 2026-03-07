# flattening-per-spec Branch Context

## Goal

Refactor pymoca's compiler to align with MLS v3.5. Introduces lexical class tree vs instance tree distinction. Rewrites name lookup, instantiation, flattening per spec.

Branch: `claude/flattening-per-spec` → force-push to `fix-inherited-symbol-scope-pr` (PR #307, issue #266).

## Current Status

- **Name lookup**: Complete for core features, manually tested against ModelicaCompliance
- **Instantiation**: Complete for core features
- **Flattening**: WIP, targeting plug-compatibility with legacy `tree.flatten_class()`

### Recent Changes That Still Need Review

- **tree/ split**: Done — monolithic `tree.py` split into `tree/` package (5 submodules)
- **Modification ordering**: Fixed for nested extends (`search_inherited=True` after first name)
- **ModelicaCompliance test automation**: see `doc/compliance-test-plan.md` for plan this was created from

## Architecture

For detailed architecture, subsystem docs, and design decisions see `doc/architecture.md`

