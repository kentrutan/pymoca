// Test: 'time' used as a modification value (built-in Modelica variable).
model TimeInModification
  Real wz = time;
  parameter Real x(start = time) = 1.0;
end TimeInModification;
