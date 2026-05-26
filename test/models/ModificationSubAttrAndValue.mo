model M
    parameter Real k(unit="V/A") = 1.0;
end M;

model Outer
    M m(k(unit="m/s") = 2.0);
end Outer;
