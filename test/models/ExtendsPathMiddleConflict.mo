// Test: an inherited symbol whose name matches a *middle* component of a
// qualified extends path must not trigger a false ambiguity error.
// Here, BaseA inherits constant "Air=2" from Base, and ExtendsOuter extends
// both BaseA and Outer.Inner.  "Air" appears at position [1] in the path
// Outer.Air.Inner but that "Air" is resolved relative to Outer, not from
// the merged scope — so it is unambiguous.
package Outer
  package Air
    model Inner
      parameter Real x = 1.0;
    end Inner;
  end Air;
end Outer;

model Base
  constant Integer Air = 2;
end Base;

model BaseA
  extends Base;
end BaseA;

model ExtendsOuter
  extends BaseA;
  extends Outer.Air.Inner;
end ExtendsOuter;
