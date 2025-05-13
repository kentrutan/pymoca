// Example from Hans Olsson comment from WIP Modelica 3.4 MCP-0019
// Intended to show the need for instantiation in composite name lookup
// https://github.com/modelica/ModelicaSpecification/issues/1829#issuecomment-435677034

model MyModel
protected
  package A
    extends B(redeclare package C=B.D);
  end A;
  package B
    package D
      type T=Real;
    end D;
    replaceable package C end C;
  end B;
  parameter A.C.T x=2;
end MyModel;

// OMC 1.25.0 -i=MyModel gives:
// class MyModel
//   protected parameter Real x = 2.0;
// end MyModel;

// Without instantiation in lookup but using the rest of the procedure from the spec,
// A.C.T is not found because A and B are only partially instantiated and C is not
// instantiated at all.
