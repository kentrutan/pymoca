// Test for the scope of modification references and values
// Derived from Modelica Spec v3.5 Section 4.5.1 "Demonstrating the differences in scopes"
// We added /* comments */
model Resistor
    parameter Real R = 1; /* Added value */
    /* ... */
end Resistor;
model A
    parameter Real R = 3; /* Added value */
    replaceable model Load=Resistor(R=R) /* constrainedby TwoPin*/ ;
    // Correct, sets the R in Resistor to R from model A.
    replaceable model LoadError
        extends Resistor(R=R);
        // Gives the singular equation R=R, since the right-hand side R
        // is searched for in LoadError and found in its base-class Resistor.
        constant Real c = 42; /* Added */
    end LoadError /* constrainedby TwoPin */;
    /* TODO: Uncomment when we support global name lookup */
    /*
    encapsulated model Load2=.Resistor(R=2); // Ok
    encapsulated model LoadR=.Resistor(R=R); // Illegal
    */
    Load a(R=4),b,c(R=R); /* Added modifications */
    LoadError d(c=LoadError.c*2); /* Added this line to instantiate LoadError*/
    /* ConstantSource ...; */
    /* ... */
end A;
/* Added the rest below */
model B
    extends A(redeclare model Load=Resistor);
    Load e, f(R=R), g;
    LoadC h(R=LoadError.c);
    LoadB i, j(R=2);
    model LoadB = Resistor(R=R);
    model LoadC = LoadB;
end B;

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
