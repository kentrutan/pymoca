// Taken from https://github.com/modelica/ModelicaSpecification/issues/1829#issuecomment-435677277
package P
  model BT
    parameter Real m = -1;
      Real x;
    equation
      x = m;
  end BT;

  model M
    model A
      replaceable model AT
        Real x;
        parameter Real m = -10;
      equation
        x = m;
      end AT;
      AT at;
    end A;

    model B
      model BT
        Real x;
        parameter Real m = 0;
      equation
        x = m;
      end BT;
      BT bt;
    end B;

    extends A(redeclare model AT = BT); // Here we expect to find P.M.B.BT, not P.BT
    extends B;
  end M;
end P;
