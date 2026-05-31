model M
  parameter Real k(unit="V/A") = 1.0;
end M;

model NestedModificationArgScope
  parameter String p_unit = "m/s";
  M m(k(unit=p_unit) = 2.0);
end NestedModificationArgScope;
