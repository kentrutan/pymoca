// Test: modifications targeting symbols added by a replaceable class's
// concrete redeclaration must not raise a spurious error.
package P
  package AbstractMedium
    replaceable record ThermodynamicState
    end ThermodynamicState;
  end AbstractMedium;

  package ConcreteMedium
    extends AbstractMedium;
    redeclare record extends ThermodynamicState
      parameter Real p = 1e5;
      parameter Real T = 300.0;
    end ThermodynamicState;
  end ConcreteMedium;

  model Example
    ConcreteMedium.ThermodynamicState state(p = 2e5, T = 350.0);
  end Example;
end P;
