# pymoca CasADi Backend — User Guide

This guide explains how to use the pymoca CasADi backend to translate Modelica models
into [CasADi](https://web.casadi.org/) symbolic expressions, and how to use those
expressions for simulation, optimization, and sensitivity analysis in Python.

**Assumed knowledge:** Python, basic Modelica (models, equations, `der()`, `parameter`).
No prior CasADi knowledge is required — this guide introduces the necessary concepts as
they arise.

---

## Table of Contents

- [pymoca CasADi Backend — User Guide](#pymoca-casadi-backend--user-guide)
  - [Table of Contents](#table-of-contents)
  - [1. Why CasADi?](#1-why-casadi)
  - [2. Installation](#2-installation)
  - [3. CasADi Crash Course for Modelica Users](#3-casadi-crash-course-for-modelica-users)
    - [3.1 Symbolic types: SX, MX, DM](#31-symbolic-types-sx-mx-dm)
    - [3.2 The `ca.Function` class](#32-the-cafunction-class)
    - [3.3 Building and solving an NLP in one screen](#33-building-and-solving-an-nlp-in-one-screen)
  - [4. Quick Start — Compiling a Modelica Model](#4-quick-start--compiling-a-modelica-model)
    - [4.1 Alternative: compile from an in-memory AST](#41-alternative-compile-from-an-in-memory-ast)
  - [5. The DAE Residual Function](#5-the-dae-residual-function)
    - [5.1 Residual form](#51-residual-form)
    - [5.2 Evaluating the residual numerically](#52-evaluating-the-residual-numerically)
    - [5.3 Initial residual](#53-initial-residual)
  - [6. Variable Metadata](#6-variable-metadata)
    - [6.1 The `Variable` class](#61-the-variable-class)
    - [6.2 `variable_metadata_function`](#62-variable_metadata_function)
  - [7. Simulation with CasADi Integrators](#7-simulation-with-casadi-integrators)
    - [7.1 Wrapping a pure ODE](#71-wrapping-a-pure-ode)
    - [7.2 Forward simulation with multiple time steps](#72-forward-simulation-with-multiple-time-steps)
    - [7.3 DAE simulation with IDAS](#73-dae-simulation-with-idas)
  - [8. Optimal Control — Direct Collocation](#8-optimal-control--direct-collocation)
    - [8.1 Multiple shooting](#81-multiple-shooting)
  - [9. Automatic Differentiation](#9-automatic-differentiation)
    - [9.1 Jacobian of the DAE residual](#91-jacobian-of-the-dae-residual)
    - [9.2 Gradient and Hessian](#92-gradient-and-hessian)
    - [9.3 Sensitivity of integration output w.r.t. parameters](#93-sensitivity-of-integration-output-wrt-parameters)
  - [10. Model Simplification Options](#10-model-simplification-options)
    - [Recommended combinations](#recommended-combinations)
    - [Calling `simplify` manually](#calling-simplify-manually)
  - [11. Caching and Code Generation](#11-caching-and-code-generation)
    - [11.1 Pickle cache](#111-pickle-cache)
    - [11.2 Native code generation](#112-native-code-generation)
    - [11.3 Manual save/load](#113-manual-saveload)
  - [12. Advanced Features](#12-advanced-features)
    - [12a. Arrays and Matrices](#12a-arrays-and-matrices)
    - [12b. Interpolation / Look-up Tables](#12b-interpolation--look-up-tables)
    - [12c. Time Delays](#12c-time-delays)
    - [12d. Component Models and Connectors](#12d-component-models-and-connectors)
  - [13. API Reference](#13-api-reference)
    - [`transfer_model`](#transfer_model)
    - [`gen_casadi.generate`](#gen_casadigenerate)
    - [`Model` class](#model-class)
    - [`Variable` class](#variable-class)
    - [`save_model` / `load_model`](#save_model--load_model)
  - [Further Reading](#further-reading)

---

## 1. Why CasADi?

[CasADi](https://web.casadi.org/) is an open-source framework for numerical optimization
and algorithmic differentiation. Its main strengths are:

- **Automatic differentiation (AD):** Exact Jacobians, Hessians, and higher-order
  derivatives computed symbolically — no finite differences, no round-off error.
  See [CasADi AD docs](https://web.casadi.org/docs/#automatic-differentiation).
- **DAE/ODE integration:** Wraps the
  [SUNDIALS](https://computing.llnl.gov/projects/sundials) solvers CVODES (ODEs) and
  IDAS (DAEs) with automatic sensitivity propagation.
  See [CasADi integrator docs](https://web.casadi.org/docs/#integrators).
- **Nonlinear programming (NLP):** Structured interface to IPOPT and other solvers for
  optimal control, parameter estimation, and trajectory optimization.
  See [CasADi NLP docs](https://web.casadi.org/docs/#nonlinear-programming).
- **Code generation:** Compiles CasADi functions to self-contained C code for deployment
  in embedded systems or high-performance servers.
  See [CasADi code generation docs](https://web.casadi.org/docs/#generating-c-code).

pymoca's CasADi backend parses a Modelica model and produces a `Model` object whose
equations are CasADi symbolic expressions. You can then hand those expressions directly
to CasADi's integrators and solvers without writing any model equations by hand.

---

## 2. Installation

```bash
pip install pymoca casadi
```

For the NLP examples you will also need [IPOPT](https://coin-or.github.io/Ipopt/),
which ships pre-compiled with the `casadi` wheel on most platforms — no separate install
is needed.

```python
# Verify
import casadi as ca
import pymoca
print(ca.__version__)   # e.g. 3.6.x
print(pymoca.__version__)
```

---

## 3. CasADi Crash Course for Modelica Users

### 3.1 Symbolic types: SX, MX, DM

CasADi has three matrix types you will encounter.
See the [CasADi symbolic types docs](https://web.casadi.org/docs/#the-sx-symbolics) for
the full picture.

| Type    | What it is                              | When you see it                        |
| ------- | --------------------------------------- | -------------------------------------- |
| `ca.MX` | Symbolic matrix expression (graph node) | pymoca uses MX for all model variables |
| `ca.SX` | Scalar-level symbolic expression        | Appears when `expand_mx=True` is set   |
| `ca.DM` | Dense numerical matrix                  | Used to pass numbers into functions    |

```python
import casadi as ca

# Create symbolic scalars and vectors
x   = ca.MX.sym("x")          # scalar
v   = ca.MX.sym("v", 3)       # column vector of length 3
M   = ca.MX.sym("M", 3, 3)    # 3×3 matrix

# Arithmetic is numpy-like
expr = ca.sin(x)**2 + ca.cos(x)**2   # simplifies symbolically

# Numerical matrices
A = ca.DM([[1, 2], [3, 4]])   # 2×2
print(A)
```

### 3.2 The `ca.Function` class

A [`ca.Function`](https://web.casadi.org/docs/#casadi-s-function-class) wraps one or
more CasADi expressions into a callable that accepts `DM` (numeric) or `MX` (symbolic)
inputs and returns outputs.

```python
x = ca.MX.sym("x")
y = ca.MX.sym("y")
f = ca.Function("f", [x, y], [x**2 + y])   # f(x, y) = x² + y

result = f(3.0, 5.0)   # DM inputs
print(float(result))   # 14.0

# Call with symbolic inputs — returns another symbolic expression
z = ca.MX.sym("z")
expr = f(z, 2*z)       # x²+y evaluated at x=z, y=2z  →  z²+2z
```

### 3.3 Building and solving an NLP in one screen

The full NLP interface is described in the
[CasADi NLP docs](https://web.casadi.org/docs/#nonlinear-programming).

```python
x = ca.MX.sym("x", 2)
nlp = {
    "x": x,
    "f": (x[0] - 1)**2 + (x[1] - 2)**2,   # objective
    "g": x[0] + x[1],                       # constraint
}
solver = ca.nlpsol("S", "ipopt", nlp, {"ipopt.print_level": 0})
sol = solver(x0=[0, 0], lbg=3, ubg=3)
print(sol["x"])   # ≈ [1, 2]
```

---

## 4. Quick Start — Compiling a Modelica Model

The main entry point is `transfer_model`, which parses the `.mo` file(s) in a folder
and returns a `Model` object.

**Modelica source** — `Spring.mo`:

```modelica
model Spring
    Real x, v_x;
    parameter Real c = 0.1;
    parameter Real k = 2;
equation
    der(x) = v_x;
    der(v_x) = -k*x - c*v_x;
end Spring;
```

**Python**:

```python
from pymoca.backends.casadi.api import transfer_model

model = transfer_model("/path/to/models", "Spring", compiler_options={})

print(model)
# Model
# states:      [x[1,1]:float, v_x[1,1]:float]
# der_states:  [der(x)[1,1]:float, der(v_x)[1,1]:float]
# parameters:  [c[1,1]:float, k[1,1]:float]
# equations:   [der(x) - v_x, der(v_x) - (-k*x - c*v_x)]
```

Every list entry is a [`Variable`](#variable-class) whose `.symbol` attribute is a
`ca.MX` symbolic. The `equations` list contains CasADi expressions in *residual form*
(the left-hand side of `LHS - RHS = 0`).

### 4.1 Alternative: compile from an in-memory AST

If you have already parsed the Modelica source yourself (e.g. to run multiple `generate`
calls on the same parse tree), you can call `generate` directly:

```python
from pymoca import parser
import pymoca.backends.casadi.generator as gen_casadi

with open("Spring.mo") as f:
    ast_tree = parser.parse(f.read())

model = gen_casadi.generate(ast_tree, "Spring")
```

---

## 5. The DAE Residual Function

### 5.1 Residual form

pymoca expresses every Modelica equation `LHS = RHS` as the residual `LHS - RHS = 0`.
The full DAE system is a `ca.Function` with signature:

```
dae_residual(t, x, ẋ, z, u, c, p)  →  r
```

| Argument | Meaning                     | CasADi symbol              |
| -------- | --------------------------- | -------------------------- |
| `t`      | time                        | scalar                     |
| `x`      | differential states         | `veccat(model.states)`     |
| `ẋ`      | state derivatives           | `veccat(model.der_states)` |
| `z`      | algebraic states            | `veccat(model.alg_states)` |
| `u`      | inputs                      | `veccat(model.inputs)`     |
| `c`      | constants                   | `veccat(model.constants)`  |
| `p`      | parameters                  | `veccat(model.parameters)` |
| `r`      | residuals (= 0 at solution) | `veccat(model.equations)`  |

This is exactly the semi-explicit DAE form used by CasADi's IDAS integrator.

### 5.2 Evaluating the residual numerically

```python
from pymoca.backends.casadi.api import transfer_model
import casadi as ca

model = transfer_model("/path/to/models", "Spring", {})

dae = model.dae_residual_function
print(dae)
# dae_residual:(i0,i1[2],i2[2],i3[0],i4[0],i5[0],i6[2])->(o0[2]) MXFunction

# Numerical evaluation: t=0, x=[1,0], ẋ=[0,0], z=[], u=[], c=[], p=[c,k]
t  = ca.DM(0.0)
x  = ca.DM([1.0, 0.0])      # x=1, v_x=0
xd = ca.DM([0.0, 0.0])      # ẋ (trial)
z  = ca.DM([])               # no algebraic states
u  = ca.DM([])               # no inputs
c  = ca.DM([])               # no constants
p  = ca.DM([0.1, 2.0])      # c=0.1, k=2.0

residual = dae(t, x, xd, z, u, c, p)
print(residual)
# [0 - 0.0 = 0, 0 - (-2*1 - 0.1*0) = 2]
# Residual is nonzero because ẋ=[0,0] is not the true derivative at (x=1, v_x=0)
```

The residual is zero when `ẋ` equals the correct time-derivative vector. To obtain the
correct `ẋ` at `t=0`, pass it as `[v_x, -k*x - c*v_x] = [0, -2]`:

```python
xd_true = ca.DM([0.0, -2.0])
print(dae(t, x, xd_true, z, u, c, p))   # [0, 0]
```

### 5.3 Initial residual

Modelica's `initial equation` section produces a separate `initial_residual_function`
with the same signature. Use it to find consistent initial conditions:

```python
init_res = model.initial_residual_function
```

If the model has no `initial equation` section, this function returns an empty vector.
You can still find consistent initial conditions by solving a small NLP that drives
`dae_residual(t=0, x0, ẋ0, z0, ...)` to zero.

---

## 6. Variable Metadata

### 6.1 The `Variable` class

Each entry in `model.states`, `model.alg_states`, `model.inputs`, `model.parameters`,
`model.constants`, etc., is a `Variable` instance with the following attributes:

| Attribute  | Type            | Meaning                                 |
| ---------- | --------------- | --------------------------------------- |
| `.symbol`  | `ca.MX`         | CasADi symbolic (use in expressions)    |
| `.value`   | float / `ca.MX` | Default / assigned value (NaN if unset) |
| `.start`   | float / `ca.MX` | Initial guess for the solver            |
| `.min`     | float / `ca.MX` | Lower bound                             |
| `.max`     | float / `ca.MX` | Upper bound                             |
| `.nominal` | float / `ca.MX` | Scaling hint                            |
| `.fixed`   | bool            | Whether the start value is fixed        |

Attributes that depend on parameters are `ca.MX` expressions instead of plain floats;
they are evaluated by `variable_metadata_function` (see §6.2).

```python
for v in model.parameters:
    print(v.symbol.name(), "default =", v.value)
# c  default = 0.1
# k  default = 2.0

for v in model.states:
    print(v.symbol.name(), "start =", v.start, "min =", v.min)
# x    start = 0  min = -inf
# v_x  start = 0  min = -inf
```

### 6.2 `variable_metadata_function`

This `ca.Function` accepts the parameter vector and returns metadata matrices for all
variable groups:

```
variable_metadata_function(p)  →  (states_meta, alg_states_meta, inputs_meta, ...)
```

Each output is an `(n_vars × 6)` matrix whose columns correspond to the six attributes
in `CASADI_ATTRIBUTES = ("value", "min", "max", "start", "fixed", "nominal")`.

This is useful when parameter-dependent bounds or start values must be evaluated at a
specific parameter point before setting up a solver.

```python
p_vec = ca.veccat(*[v.symbol for v in model.parameters])
meta_fn = model.variable_metadata_function

# Evaluate at default parameter values
p_vals = ca.DM([v.value for v in model.parameters])
meta = meta_fn(p_vals)
# meta[0] is the states metadata matrix
```

---

## 7. Simulation with CasADi Integrators

CasADi's `integrator` function wraps CVODES (for ODEs) and IDAS (for index-1 DAEs).
Both propagate sensitivity equations alongside the main system — a feature central to
parameter estimation and optimal control.

Full documentation:
[CasADi integrators](https://web.casadi.org/docs/#integrators).

### 7.1 Wrapping a pure ODE

For a model with no algebraic states (`model.alg_states == []`), use the `cvodes`
integrator:

```python
from pymoca.backends.casadi.api import transfer_model
import casadi as ca
import numpy as np

model = transfer_model("/path/to/models", "Spring", {})

# Extract symbols for convenience
x_sym  = ca.veccat(*[v.symbol for v in model.states])      # [x, v_x]
xd_sym = ca.veccat(*[v.symbol for v in model.der_states])  # [ẋ, v̇_x]
p_sym  = ca.veccat(*[v.symbol for v in model.parameters])  # [c, k]
t_sym  = model.time

# dae_residual has 7 inputs: (t, x, ẋ, z, u, c, p)
# For CVODES we express it as ẋ = f(t, x, p).
# Because there are no z or u, we can call directly.
dae = model.dae_residual_function

# Build the ode dict that CVODES expects:
# ode = dẋ expressed as a function of (t, x, p)
# We solve: residual = xd - f(t,x,p) = 0  =>  xd = f
# CasADi needs the RHS f, not the residual.  Use ca.rootfinder or just
# differentiate: for explicit ODEs we can extract the RHS symbolically.

# For explicit ODEs the residual is r = ẋ - f(x,p,t), so f = ẋ - r(ẋ=0)
r_at_zero = dae(t_sym, x_sym, ca.DM.zeros(x_sym.size1()), ca.DM([]),
                ca.DM([]), ca.DM([]), p_sym)
ode_rhs = -r_at_zero   # f(t, x, p) = ẋ  at ẋ=0

ode_dict = {"x": x_sym, "p": p_sym, "t": t_sym, "ode": ode_rhs}

# Create integrator: integrate from t=0 to t=T
T = 5.0
integrator = ca.integrator("I", "cvodes", ode_dict, 0.0, T)

# Initial conditions and parameter values
x0_vals = ca.DM([1.0, 0.0])   # x=1, v_x=0
p_vals  = ca.DM([0.1, 2.0])   # c=0.1, k=2.0

sol = integrator(x0=x0_vals, p=p_vals)
print(sol["xf"])   # state at t=T
```

### 7.2 Forward simulation with multiple time steps

CasADi integrators accept a `grid` option to return the solution at multiple time
points:

```python
N = 100
t_grid = np.linspace(0, T, N + 1).tolist()

integrator = ca.integrator("I", "cvodes", ode_dict, 0.0, t_grid)
sol = integrator(x0=x0_vals, p=p_vals)
# sol["xf"] is a (2 × N) matrix — states at each grid point
```

### 7.3 DAE simulation with IDAS

For models that contain algebraic states (`model.alg_states != []`), use the `idas`
integrator.  The IDAS integrator expects the DAE in residual form:
`F(t, x, ẋ, z, p) = 0`.

```python
from pymoca.backends.casadi.api import transfer_model
import casadi as ca

model = transfer_model("/path/to/models", "Estimator", {})

# Estimator.mo:
#   der(x) = -x;   (ODE)
#   y = x;         (algebraic output)

x_sym  = ca.veccat(*[v.symbol for v in model.states])
xd_sym = ca.veccat(*[v.symbol for v in model.der_states])
z_sym  = ca.veccat(*[v.symbol for v in model.alg_states])
t_sym  = model.time

# dae_residual takes (t, x, ẋ, z, u, c, p)
# IDAS needs a function f(t,x,ẋ,z,p) that returns residuals
dae_fn = model.dae_residual_function

dae_dict = {
    "x": x_sym,
    "z": z_sym,
    "t": t_sym,
    "ode": dae_fn(t_sym, x_sym, xd_sym, z_sym,
                  ca.DM([]), ca.DM([]), ca.DM([])),
    # For IDAS pass alg residuals in "alg" key
}
# Note: see CasADi docs for the precise idas dict structure.
# https://web.casadi.org/docs/#integrators
```

> **Tip:** The [CasADi IDAS documentation](https://web.casadi.org/docs/#integrators)
> lists all options including `abstol`, `reltol`, `max_num_steps`, etc.

---

## 8. Optimal Control — Direct Collocation

This section shows how to set up an optimal control problem using the DAE residual as a
constraint.  The approach is called *direct collocation* (here backward Euler, matching
the notebook in `test/notebooks/Casadi.ipynb`).

**Model** — `Exponential.mo`:

```modelica
model Exponential
    Real x(start = 0.0);
    input Real u;
equation
    der(x) = -x + u;
end Exponential;
```

**Goal:** drive `x` to 1 over `N` steps by choosing `u`, subject to `0 ≤ u ≤ 1.75`.

```python
from pymoca.backends.casadi.api import transfer_model
import casadi as ca
import numpy as np

model = transfer_model("/path/to/models", "Exponential", {})
dae = model.dae_residual_function

# Starting value of x
X0 = model.states[0].start   # 0.0

# Discretization
N  = 10     # number of collocation steps
dt = 0.1    # time step

# --- Decision variables ---
X = ca.MX.sym("X", N)          # state at each step
U = ca.MX.sym("U", N)          # control at each step
lbX = np.full(N, -np.inf);  ubX = np.full(N, np.inf)
lbU = np.full(N, 0.0);      ubU = np.full(N, 1.75)

# --- Collocation constraints (backward Euler) ---
# dae_residual(t, x, ẋ, z, u, c, p) == 0
# ẋ ≈ (x[i] - x[i-1]) / dt
g = []
# Step 0: ẋ ≈ (X[0] - X0) / dt
res = dae(0, X[0], (X[0] - X0) / dt, ca.MX(), U[0], ca.MX(), ca.MX())
g.append(res)
for i in range(1, N):
    res = dae(i * dt, X[i], (X[i] - X[i - 1]) / dt, ca.MX(), U[i], ca.MX(), ca.MX())
    g.append(res)

# --- Objective: drive X[-1] → 1 ---
f = (X[-1] - 1.0)**2

# --- Solve NLP ---
# See https://web.casadi.org/docs/#nonlinear-programming
nlp = {
    "x": ca.vertcat(X, U),
    "f": f,
    "g": ca.vertcat(*g),
}
solver = ca.nlpsol("nlp", "ipopt", nlp, {"ipopt": {"tol": 1e-6}})

solution = solver(
    lbx=ca.vertcat(lbX, lbU),
    ubx=ca.vertcat(ubX, ubU),
    lbg=0,
    ubg=0,
)

X_opt = np.array(solution["x"][:N]).flatten()
U_opt = np.array(solution["x"][N:]).flatten()
print("Final state:", X_opt[-1])   # ≈ 1.0
```

### 8.1 Multiple shooting

For longer horizons, *multiple shooting* is more numerically robust than collocation.
The idea is to integrate the DAE over each interval and stitch the segments together
with continuity constraints.  CasADi's `integrator` returns a differentiable map
from `x0` to `xf` that IPOPT can differentiate through automatically.

See the CasADi example
[`direct_multiple_shooting.py`](https://github.com/casadi/casadi/blob/main/docs/examples/python/direct_multiple_shooting.py)
for a complete implementation.

---

## 9. Automatic Differentiation

CasADi provides exact Jacobians and Hessians of any `ca.Function` for free, by
replaying the computation graph in forward or reverse mode.

Full documentation:
[CasADi AD docs](https://web.casadi.org/docs/#automatic-differentiation).

### 9.1 Jacobian of the DAE residual

```python
from pymoca.backends.casadi.api import transfer_model
import casadi as ca

model = transfer_model("/path/to/models", "Spring", {})
dae = model.dae_residual_function

# Symbolic inputs
x_sym  = ca.veccat(*[v.symbol for v in model.states])
xd_sym = ca.veccat(*[v.symbol for v in model.der_states])
p_sym  = ca.veccat(*[v.symbol for v in model.parameters])

# Evaluate residual symbolically
r = dae(model.time, x_sym, xd_sym, ca.DM([]), ca.DM([]), ca.DM([]), p_sym)

# Jacobian dr/dx (∂residual / ∂states)
J_x  = ca.jacobian(r, x_sym)

# Jacobian dr/dẋ (mass/damping matrix, "E" matrix in E·ẋ = f)
J_xd = ca.jacobian(r, xd_sym)

print(ca.Function("J_x",  [x_sym, xd_sym, p_sym], [J_x])(
    ca.DM([1.0, 0.0]), ca.DM([0.0, -2.0]), ca.DM([0.1, 2.0])
))
```

### 9.2 Gradient and Hessian

```python
x = ca.MX.sym("x", 2)
f_expr = x[0]**2 + x[1]**2

grad_f = ca.gradient(f_expr, x)     # ∇f
hess_f, _ = ca.hessian(f_expr, x)   # H_f

grad_fn = ca.Function("grad", [x], [grad_f])
hess_fn = ca.Function("hess", [x], [hess_f])

print(grad_fn(ca.DM([3.0, 4.0])))   # [6, 8]
print(hess_fn(ca.DM([3.0, 4.0])))   # [[2, 0], [0, 2]]
```

### 9.3 Sensitivity of integration output w.r.t. parameters

CVODES and IDAS compute forward sensitivities `dx_f/dp` as part of the integration step
at essentially no extra cost.  This is central to gradient-based parameter estimation:

```python
# Build integrator as in §7.1
# The integrator's output 'xf' is differentiable w.r.t. 'p'
J_p = ca.jacobian(sol["xf"], p_sym)
sens_fn = ca.Function("sens", [x_sym, p_sym], [J_p])
```

See [CasADi sensitivity analysis](https://web.casadi.org/docs/#sensitivity-analysis)
for the full treatment.

---

## 10. Model Simplification Options

After compilation, `model.simplify(options)` (called automatically by `transfer_model`)
applies a chain of symbolic transformations controlled by a dictionary of options.

All options and their defaults are listed in
`src/pymoca/backends/casadi/_options.py`:

| Option                           | Default | Description                                           |
| -------------------------------- | ------- | ----------------------------------------------------- |
| `library_folders`                | `[]`    | Additional folders to scan for `.mo` files            |
| `verbose`                        | `False` | Print extra diagnostics                               |
| `check_balanced`                 | `True`  | Warn if `#equations ≠ #unknowns`                      |
| `mtime_check`                    | `True`  | Invalidate cache when source `.mo` is newer           |
| `cache`                          | `False` | Save/load compiled model to `.pymoca_cache`           |
| `codegen`                        | `False` | Compile CasADi functions to a native shared library   |
| `expand_mx`                      | `False` | Expand all MX nodes to scalar SX (implied by `cache`) |
| `unroll_loops`                   | `True`  | Unroll Modelica `for` loops into scalar equations     |
| `inline_functions`               | `True`  | Inline Modelica function calls                        |
| `expand_vectors`                 | `False` | Scalarize vector-valued variables                     |
| `resolve_parameter_values`       | `False` | Propagate constant parameter values                   |
| `replace_parameter_expressions`  | `False` | Substitute parameter-dependent expressions            |
| `replace_constant_expressions`   | `False` | Substitute constant-dependent expressions             |
| `eliminate_constant_assignments` | `False` | Remove trivial constant equations                     |
| `replace_parameter_values`       | `False` | Replace all parameter symbols with their values       |
| `replace_constant_values`        | `False` | Replace all constant symbols with their values        |
| `eliminable_variable_expression` | `None`  | Callable to mark extra eliminable variables           |
| `factor_and_simplify_equations`  | `False` | Apply algebraic factoring/simplification              |
| `detect_aliases`                 | `False` | Eliminate variables related by `x = ±y`               |
| `allow_derivative_aliases`       | `True`  | Include `der(x) = ±der(y)` in alias detection         |
| `reduce_affine_expression`       | `False` | Factor affine equations into matrix form              |

### Recommended combinations

**Fast simulation (caching + native code):**
```python
options = {
    "cache": True,
    "replace_parameter_values": True,
    "detect_aliases": True,
}
model = transfer_model("/path/to/models", "Spring", options)
```

**NLP / optimal control (scalarize for IPOPT):**
```python
options = {
    "expand_vectors": True,
    "detect_aliases": True,
    "replace_constant_values": True,
    "inline_functions": True,
}
model = transfer_model("/path/to/models", "Spring", options)
```

**Symbolic analysis (preserve structure):**
```python
options = {}   # defaults — keep all parameters symbolic
model = transfer_model("/path/to/models", "Spring", options)
```

### Calling `simplify` manually

You can also apply simplifications to an already-compiled model:

```python
import pymoca.backends.casadi.generator as gen_casadi
from pymoca import parser

with open("MyModel.mo") as f:
    ast = parser.parse(f.read())

model = gen_casadi.generate(ast, "MyModel")
model.simplify({"detect_aliases": True, "replace_constant_values": True})
```

---

## 11. Caching and Code Generation

For production use, re-parsing and re-compiling a large Modelica model on every Python
launch is wasteful.  pymoca offers two persistence mechanisms.

### 11.1 Pickle cache

```python
options = {"cache": True}
model = transfer_model("/path/to/models", "Aircraft", options)
```

On the **first call**, pymoca compiles the model and serializes all four CasADi
functions (`dae_residual`, `initial_residual`, `variable_metadata`,
`delay_arguments`) to a single file `Aircraft.pymoca_cache` using Python's `pickle`.

On **subsequent calls**, the cache is loaded directly — skipping the ANTLR parser and
CasADi symbolic build.  If any `.mo` file in the folder is newer than the cache, the
cache is invalidated and the model is recompiled.

> **Note:** `cache=True` implies `expand_mx=True` because MX functions cannot be
> pickled; they are first expanded to SX.

### 11.2 Native code generation

```python
options = {"codegen": True}
model = transfer_model("/path/to/models", "Aircraft", options)
```

`codegen=True` goes further: CasADi's
[CodeGenerator](https://web.casadi.org/docs/#generating-c-code) emits C source code for
each function, which is then compiled to a shared library (`.so` on Linux/macOS,
`.dll` on Windows) using the system C compiler.

The generated library includes:
- The nondifferentiated function
- The Jacobian-times-vector product (forward mode)
- The vector-times-Jacobian product (reverse mode)
- The Hessian-times-vector product

This makes repeated function evaluations (as in a large OCP) substantially faster.
Codegen and cache are mutually exclusive; if both are set, codegen takes precedence.

### 11.3 Manual save/load

```python
from pymoca.backends.casadi.api import save_model, load_model

save_model("/path/to/models", "Aircraft", model, options)
cached = load_model("/path/to/models", "Aircraft", options)
```

`load_model` raises `InvalidCacheError` if the cache is stale, was generated with
different options, or targets an incompatible CasADi version.

---

## 12. Advanced Features

### 12a. Arrays and Matrices

Modelica array variables map to CasADi MX column vectors or matrices.

**Modelica:**
```modelica
model ArrayExample
    Real a[3];
    Real b[3];
equation
    b = a + {1, 2, 3};
end ArrayExample;
```

**Python — after compilation:**
```python
model = transfer_model("/path/to/models", "ArrayExample", {})

a_var = model.alg_states[0]
print(a_var.symbol.shape)   # (3, 1)
```

The `expand_vectors` option scalarizes these into individual `a[1]`, `a[2]`, `a[3]`
symbols when a solver requires scalar-only inputs.

Matrix operations (`*` for element-wise, `ca.mtimes` for matrix multiply) follow
CasADi conventions:

```python
A = ca.MX.sym("A", 3, 3)
b = ca.MX.sym("b", 3)

c = ca.mtimes(A, b)    # A·b  (matrix–vector product)
d = ca.mtimes(A.T, b)  # Aᵀ·b
```

See the
[CasADi matrix operations docs](https://web.casadi.org/docs/#matrix-operations).

### 12b. Interpolation / Look-up Tables

pymoca maps Modelica's `_pymoca_interp1d` and `_pymoca_interp2d` built-ins to
[`ca.interpolant`](https://web.casadi.org/docs/#using-lookup-tables), which provides
differentiable linear interpolation.

**Modelica:**
```modelica
model Interpolate
    parameter Real xp[3] = {0.0, 1.0, 2.0};
    parameter Real yp[3] = {0.0, 1.0, 4.0};
    Real x, y;
equation
    y = _pymoca_interp1d(xp, yp, x);
end Interpolate;
```

**Python — the compiled function is differentiable:**
```python
model = transfer_model("/path/to/models", "Interpolate", {})

# The interpolant appears embedded in the residual as a ca.Function call.
# You can differentiate through it just like any other expression.
dae = model.dae_residual_function
x_sym = model.alg_states[0].symbol
r = dae(0, ca.DM([]), ca.DM([]), ca.vertcat(*[v.symbol for v in model.alg_states]),
        ca.DM([]), ca.DM([]), ca.veccat(*[v.symbol for v in model.parameters]))

dy_dx = ca.jacobian(r, x_sym)
```

See the
[CasADi interpolant documentation](https://web.casadi.org/docs/#using-lookup-tables)
for available interpolation methods (`"linear"`, `"bspline"`) and how to create
stand-alone look-up tables.

### 12c. Time Delays

Modelica's `delay(x, τ)` operator is not directly representable as an ordinary
algebraic expression in CasADi.  pymoca handles this by introducing *delay state*
pseudo-inputs that your solver must supply externally.

**Modelica:**
```modelica
model Delay
    Real x, y;
    parameter Real hour = 3600;
equation
    y = delay(x, 6 * hour);
end Delay;
```

**After compilation:**

```python
model = transfer_model("/path/to/models", "Delay", {})

# model.delay_states   — list of MX symbols for delayed values
# model.delay_arguments — list of DelayArgument(expr, duration) namedtuples
# model.delay_arguments_function — ca.Function that evaluates expressions/durations

print(model.delay_states)       # [_pymoca_delay_0]
print(model.delay_arguments)    # [DelayArgument(expr=x, duration=6*hour)]
```

These appear in `model.inputs`, so the DAE residual function accepts them as part of
the `u` argument.  In an online simulation loop, you must maintain a ring buffer of
past state values and feed the appropriate delayed sample as an input at each step.

### 12d. Component Models and Connectors

Modelica connector variables are flattened with a dotted naming convention:

```modelica
// ConnectorHQ.mo — connector with pressure head H and flow Q
connector HQPort
    Real H;
    flow Real Q;
end HQPort;

model Pipe
    HQPort up, down;
equation
    up.H = down.H;
    up.Q + down.Q = 0;
end Pipe;
```

After `gen_casadi.generate(ast, "System")`, the variables are named
`a.up.H`, `a.up.Q`, etc. and appear as scalar `ca.MX` algebraic state symbols:

```python
for v in model.alg_states:
    print(v.symbol.name())   # "a.up.H", "a.down.Q", ...
```

This naming convention lets you select specific subsystem variables by name when
constructing larger optimization problems.

---

## 13. API Reference

### `transfer_model`

```python
from pymoca.backends.casadi.api import transfer_model

model = transfer_model(
    model_folder: str,       # directory containing .mo files
    model_name:   str,       # top-level class name to compile
    compiler_options: dict,  # see §10 for all keys (optional, default = {})
) -> Model
```

Scans `model_folder` (and `compiler_options["library_folders"]`) for `.mo` files,
parses them all, generates the CasADi model, applies simplifications, and returns a
`Model`.  If `cache=True` or `codegen=True`, also reads/writes the `.pymoca_cache` file.

> **Tip:** The `model_folder` should only contain `.mo` files that are valid Modelica
> *and* only contain Modelica features supported by the version of Pymoca that you are
> using. Otherwise, `transfer_model()` may fail.

### `gen_casadi.generate`

```python
import pymoca.backends.casadi.generator as gen_casadi
model = gen_casadi.generate(ast_tree, model_name, compiler_options={})
```

Lower-level entry point.  Useful when you already hold a parsed AST, e.g., when
compiling multiple models from the same source.

### `Model` class

`src/pymoca/backends/casadi/model.py`

| Attribute           | Type                   | Description                               |
| ------------------- | ---------------------- | ----------------------------------------- |
| `time`              | `ca.MX`                | Symbolic time variable                    |
| `states`            | `list[Variable]`       | Differential state variables              |
| `der_states`        | `list[Variable]`       | Time derivatives of states                |
| `alg_states`        | `list[Variable]`       | Algebraic state variables                 |
| `inputs`            | `list[Variable]`       | Input variables (incl. delay states)      |
| `outputs`           | `list[Variable]`       | Output variables                          |
| `parameters`        | `list[Variable]`       | Parameter variables                       |
| `constants`         | `list[Variable]`       | Constant variables                        |
| `string_parameters` | `list[StringVariable]` | String parameters                         |
| `string_constants`  | `list[StringVariable]` | String constants                          |
| `equations`         | `list[ca.MX]`          | Residual expressions (length = #unknowns) |
| `initial_equations` | `list[ca.MX]`          | Residuals from `initial equation` section |
| `delay_states`      | `list[ca.MX]`          | Delay pseudo-state symbols                |
| `delay_arguments`   | `list[DelayArgument]`  | `(expr, duration)` pairs                  |
| `alias_relation`    | `AliasRelation`        | Detected alias pairs                      |

**Key functions / properties:**

| Property / Method            | Description                                             |
| ---------------------------- | ------------------------------------------------------- |
| `dae_residual_function`      | `ca.Function(t, x, ẋ, z, u, c, p) → r`                  |
| `initial_residual_function`  | Same signature; uses `initial equation` section         |
| `variable_metadata_function` | `ca.Function(p) → (states_meta, ...)`                   |
| `delay_arguments_function`   | `ca.Function(t, x, ẋ, z, u, c, p) → (expr₁, dur₁, ...)` |
| `check_balanced()`           | Warns if `#equations ≠ #unknowns`                       |
| `simplify(options)`          | Apply simplification pipeline (see §10)                 |

### `Variable` class

| Attribute     | Type            | Description                                  |
| ------------- | --------------- | -------------------------------------------- |
| `symbol`      | `ca.MX`         | Symbolic variable; use in CasADi expressions |
| `value`       | float / `ca.MX` | Default assigned value (NaN if none)         |
| `start`       | float / `ca.MX` | Initial guess / start value                  |
| `min`         | float / `ca.MX` | Lower bound                                  |
| `max`         | float / `ca.MX` | Upper bound                                  |
| `nominal`     | float / `ca.MX` | Scaling hint                                 |
| `fixed`       | bool            | Whether start is fixed (not free)            |
| `python_type` | type            | `float`, `int`, or `bool`                    |
| `aliases`     | set             | Set of alias variable names                  |

### `save_model` / `load_model`

```python
from pymoca.backends.casadi.api import save_model, load_model

save_model(model_folder, model_name, model, compiler_options)
model = load_model(model_folder, model_name, compiler_options)  # → CachedModel
```

`InvalidCacheError` is raised when the cache is stale, version-mismatched, or
OS-incompatible (for codegen caches).

---

## Further Reading

- [CasADi documentation](https://web.casadi.org/docs/) — the primary reference for all
  CasADi functionality used above.
- [CasADi Python API reference](https://web.casadi.org/python-api/)
- [CasADi GitHub repository](https://github.com/casadi/casadi) — source, examples, and
  issue tracker.
- [CasADi examples](https://github.com/casadi/casadi/tree/main/docs/examples/python) —
  `simple_nlp.py`, `direct_multiple_shooting.py`, `idas.py`, `nlp_sensitivities.py`,
  `rocket.py`.
- [pymoca test suite](../test/gen_casadi_test.py) — contains a comprehensive set of
  worked examples covering every feature described in this guide.
- [pymoca OCP notebook](../test/notebooks/Casadi.ipynb) — interactive Jupyter example
  of the direct collocation OCP from §8.
