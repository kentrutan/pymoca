// Regression test: encapsulated function inside a record imports its enclosing
// package to type its inputs (MLS §13.2 import clauses).
// Before the fix, _get_common_parent returned an empty children string when the
// import target was itself an ancestor of the scope; _find_composite_name("", pkg)
// returned None, so the import silently failed and the input type was not found.
package Frames
  record Orientation
    Real T[3, 3];

    encapsulated function equalityConstraint
      import Frames;
      input Frames.Orientation R1;
      input Frames.Orientation R2;
      output Real residue[3];
    algorithm
      residue := {0.0, 0.0, 0.0};
    end equalityConstraint;
  end Orientation;
end Frames;

model UseOrientation
  Frames.Orientation R;
end UseOrientation;
