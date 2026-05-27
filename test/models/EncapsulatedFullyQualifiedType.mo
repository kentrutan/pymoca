// Regression test: inside an encapsulated class, a fully-qualified type name
// whose first component is a root-level package must resolve correctly.
// MLS §13.2.3 allows encapsulated classes to use globally-rooted composite names.
package P
  package Units
    package SI
      type Position = Real;
    end SI;
  end Units;

  encapsulated model EncapsulatedModel
    parameter P.Units.SI.Position x = 1.0;
  end EncapsulatedModel;
end P;
