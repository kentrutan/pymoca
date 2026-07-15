// Inside an encapsulated class, root-level names are not visible (MLS 5.3.1);
// globally-rooted names (MLS 5.3.3) and imports (MLS 13.2.1) still resolve.
package P
  package Units
    package SI
      type Position = Real;
    end SI;
  end Units;

  encapsulated model GlobalName
    parameter .P.Units.SI.Position x = 1.0;
  end GlobalName;

  encapsulated model ImportedName
    import P.Units.SI;
    parameter SI.Position x = 1.0;
  end ImportedName;

  encapsulated model LeakedRootName
    parameter P.Units.SI.Position x = 1.0;
  end LeakedRootName;
end P;
