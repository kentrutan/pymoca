// Regression test: parameter dimension computed via div() built-in.
// Before the fix, div was not in ExpressionEvaluator.binary_operator, so
// 'n = div(shift, resolution)' stayed as an unevaluated Expression. The
// downstream dimension 'buf[n+1]' then tried arithmetic on the Expression
// value and raised ModelicaSemanticError.
package P
  model Buffer
    parameter Integer shift(min = 0) = 1;
    parameter Integer resolution(min = 1) = 1;
  protected
    parameter Integer n = div(shift, resolution);
    Real buf[n + 1](each start = 0.0);
  equation
    for i in 1:n + 1 loop
      buf[i] = 0.0;
    end for;
  end Buffer;

  model Example
    Buffer b(shift = 2, resolution = 2);
  end Example;
end P;
