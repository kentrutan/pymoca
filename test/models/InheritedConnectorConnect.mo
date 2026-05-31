package P
  connector Flange
    Real s;
    flow Real f;
  end Flange;

  partial model PartialRigid
    Flange flange_a;
    Flange flange_b;
  end PartialRigid;

  model Mass
    extends PartialRigid;
  end Mass;

  model System
    Mass m;
    Flange port;
  equation
    connect(port, m.flange_a);
  end System;
end P;
