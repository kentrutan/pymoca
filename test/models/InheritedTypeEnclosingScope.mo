package P
  package Types
    type Density = Real(unit="kg/m3");
  end Types;

  partial package PartialMedium
    extends Types;
    replaceable record ThermodynamicState
    end ThermodynamicState;
  end PartialMedium;

  partial package Base
    extends PartialMedium;
    record Normal "normal record in Base"
      Density d;
    end Normal;
  end Base;

  package Concrete
    extends Base;
  end Concrete;

  model M
    Concrete.Normal s;
  end M;
end P;
