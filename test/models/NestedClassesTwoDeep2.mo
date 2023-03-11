// Example to test inherited name lookup similar to MSL v4 Resistor
package P
  package T
    class Voltage = Real(nominal=1);
  end T;
  package I
    class Level1
      // i.e. OnePort
      extends Level2(p(v(nominal=10)), n(v(nominal=10)));
    end Level1;
    class Level2
      // i.e. TwoPin
      Level3 p(v(nominal=20)), n(v(nominal=20));
    end Level2;
    class Level3
      // i.e. PostitivePin
      T.Voltage v(nominal=30);
    end Level3;
  end I;
  model M
    // i.e. Resistor
    extends I.Level1(p(v(nominal=0)), n(v(nominal=0)));
  end M;
end P;