package P
  connector Flange
    Real phi;
    flow Real tau;
  end Flange;

  partial model PartialA
    Flange flange_a;
  end PartialA;

  model B
    extends PartialA;
    Real w;
  equation
    w = der(flange_a.phi);
  end B;
end P;
