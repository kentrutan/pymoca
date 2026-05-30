package P
  partial package PartialMedium
    replaceable record ThermodynamicState
    end ThermodynamicState;
  end PartialMedium;

  partial package Base
    extends PartialMedium;
    redeclare record extends ThermodynamicState "state"
      Real d;
      Real T;
    end ThermodynamicState;
  end Base;

  package Concrete
    extends Base;
  end Concrete;

  model M
    Concrete.ThermodynamicState s;
  end M;
end P;
