package P "Example in MLS 3.5, section 7.3.1"
// Tests the `redeclare class extends` mechanism
// See /* comments */ for additions

partial package PartialMedium "Generic medium interface"
  constant Integer nX "number of substances";
  replaceable partial model BaseProperties
    Real X[nX];
    Real T; /* Added because it's referenced below */
    // ...
  end BaseProperties;

  replaceable partial function dynamicViscosity
    input Real p;
    output Real eta;
    // ...
  end dynamicViscosity;
end PartialMedium;

package MoistAir "Special type of medium"
  extends PartialMedium(nX=2);

  redeclare model extends BaseProperties(T(stateSelect = StateSelect.prefer))
    // replaces BaseProperties by a new implementation and
    // extends from Baseproperties with modification
    // note, nX = 2 (!)
  equation
    X = {0, 1};
    // ...
  end BaseProperties;

  redeclare function extends dynamicViscosity
    // replaces dynamicViscosity by a new implementation and
    // extends from dynamicViscosity
  algorithm
    eta := 2 * p;
  end dynamicViscosity;
end MoistAir;

package MoistAir2 "Alternative definition that does not work"
  extends PartialMedium(nX=2,
    redeclare model BaseProperties = MoistAir_BaseProperties,
    redeclare function dynamicViscosity = MoistAir_dynamicViscosity);

  model MoistAir_BaseProperties
    // wrong model since nX has no value
    extends PartialMedium.BaseProperties;
  equation
    X = {1, 0};
  end MoistAir_BaseProperties;

  function MoistAir_dynamicViscosity
    extends PartialMedium.dynamicViscosity;
  algorithm
    eta := p;
  end MoistAir_dynamicViscosity;
end MoistAir2;

/* Below is not in the MLS example, added to test flattening */
model TestOK
  MoistAir.BaseProperties ma;
  Real eta;
equation
    eta = MoistAir.dynamicViscosity(p=1);
end TestOK;

/* OMC 1.25.0 -i=P.TestOK gives:
class P.TestOK
  Real ma.X[1];
  Real ma.X[2];
  Real ma.T(stateSelect = StateSelect.prefer);
  Real eta;
equation
  ma.X[1] = 0.0;
  ma.X[2] = 1.0;
  eta = 2.0;
end P.TestOK;
*/

model TestFail
  MoistAir2.MoistAir_BaseProperties ma2;
end TestFail;

/* OMC 1.25.0 -i=P.TestFail gives:
RedeclareClassExtends.mo:46:5-46:41:writable] Error: PartialMedium is partial, name lookup is not allowed in partial classes.
*/
end P;
