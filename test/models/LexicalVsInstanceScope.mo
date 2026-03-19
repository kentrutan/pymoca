// Example from Modelica Spec issue #2263 about v3.4 lookup in instance vs lexical scope
// https://github.com/modelica/ModelicaSpecification/issues/2263#issue-377203249
package P

package A /* This was `model` in the original example */
  model B
    constant Integer n=1;
  equation
    assert(n==B.n, "n is not equal to B.n");
  end B;
end A;

class C = A.B(n=3);

end P;

// OMC 1.25.0 -i=P.C gives:
// class P.C
//   constant Integer n = 3;
// equation
//   assert(false, "n is not equal to B.n");
// end P.C;
