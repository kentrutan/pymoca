connector Pin
    Real v;
    flow Real i;
end Pin;

model Comp
    Pin p;
end Comp;

model IndexedConnect
    "Test connect with indices on a non-final reference segment"
    Comp a[2];
    Comp b[3];
equation
    connect(a[1].p, b[2].p);
end IndexedConnect;
