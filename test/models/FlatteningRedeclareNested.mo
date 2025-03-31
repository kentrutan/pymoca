// Example from Elena Shmoylova comment from WIP Modelica 3.4 MCP-0019
// Trying to understand overall instantiation and flattening process for Modelica 3.4
// https://github.com/modelica/ModelicaSpecification/issues/1829#issuecomment-435677277

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

    extends A(redeclare model AT = BT);
    extends B;
  end M;
end P;

// OMC 1.25.0 -i=P.M gives:
// class P.M
//   Real at.x;
//   parameter Real at.m = 0.0;
//   Real bt.x;
//   parameter Real bt.m = 0.0;
// equation
//   at.x = at.m;
//   bt.x = bt.m;
// end P.M;
