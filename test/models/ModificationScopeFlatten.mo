// Test for the scope of modification references and values
// Derived from Modelica Spec v3.5 Section 4.5.1 "Demonstrating the differences in scopes"
// We added /* comments */
model Resistor
    parameter Real R = 1; /* Added value */
    /* ... */
end Resistor;
model A
    package Const /* Added */
        constant Real c = 42;
    end Const;
    parameter Real R = 3; /* Added value */
    replaceable model Load=Resistor(R=R) /* constrainedby TwoPin*/ ;
    // Correct, sets the R in Resistor to R from model A.
    replaceable model LoadError
        extends Resistor(R=R);
        // Gives the singular equation R=R, since the right-hand side R
        // is searched for in LoadError and found in its base-class Resistor.
        constant Real c = 4; /* Added */
    end LoadError /* constrainedby TwoPin */;
    /* TODO: Uncomment when we support global name lookup */
    /*
    encapsulated model Load2=.Resistor(R=2); // Ok
    encapsulated model LoadR=.Resistor(R=R); // Illegal
    */
    Load a(R=4),b,c(R=R); /* Added modifications */
    LoadError d(c=Const.c*2); /* Added this line to instantiate LoadError*/
    /* ConstantSource ...; */
    /* ... */
end A;
/* Added the rest below */
model B
    extends A(redeclare model Load=Resistor);
    Load e, f(R=R), g;
    LoadC h(R=Const.c);
    LoadB i, j(R=2);
    model LoadB = Resistor(R=R);
    model LoadC = LoadB;
end B;
model M
    // Two instances of B: references to inherited R must stay per-instance
    B p(R=5);
    B q(R=7);
end M;

// OMC 1.25.0 -i=B gives:
// class B
//   parameter Real R = 3.0;
//   parameter Real a.R = 4.0;
//   parameter Real b.R = R;
//   parameter Real c.R = R;
//   parameter Real d.R = d.R;
//   constant Real d.c = 84.0;
//   parameter Real e.R = R;
//   parameter Real f.R = R;
//   parameter Real g.R = R;
//   parameter Real h.R = 42.0;
//   parameter Real i.R = R;
//   parameter Real j.R = 2.0;
// end B;
//
// OMC 1.22.0 simluation gives:
//  R = 3.0
//  a.R = 4.0
//  b.R = 3
//  c.R = 3
//  d.R = 0 (log shows 0.0 = 0 residual equation)
//  e.R = 3
//  f.R = 3
//  g.R = 3
//  h.R = 42.0
//  i.R = 3
//  j.R = 2.0
//
// OMC 1.25.0 -i=M gives:
// class M
//   parameter Real p.R = 5.0;
//   parameter Real p.a.R = 4.0;
//   parameter Real p.b.R = p.R;
//   parameter Real p.c.R = p.R;
//   parameter Real p.d.R = p.d.R;
//   constant Real p.d.c = 84.0;
//   parameter Real p.e.R = p.R;
//   parameter Real p.f.R = p.R;
//   parameter Real p.g.R = p.R;
//   parameter Real p.h.R = 42.0;
//   parameter Real p.i.R = p.R;
//   parameter Real p.j.R = 2.0;
//   parameter Real q.R = 7.0;
//   parameter Real q.a.R = 4.0;
//   parameter Real q.b.R = q.R;
//   parameter Real q.c.R = q.R;
//   parameter Real q.d.R = q.d.R;
//   constant Real q.d.c = 84.0;
//   parameter Real q.e.R = q.R;
//   parameter Real q.f.R = q.R;
//   parameter Real q.g.R = q.R;
//   parameter Real q.h.R = 42.0;
//   parameter Real q.i.R = q.R;
//   parameter Real q.j.R = 2.0;
// end M;
