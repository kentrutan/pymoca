connector Pin
    Real v;
    flow Real i;
end Pin;

model ArrayConnectParam
    "Test connect with parameter index in array connector"
    parameter Integer n = 2;
    Pin a[3];
    Pin b;
equation
    connect(a[n], b);
end ArrayConnectParam;
