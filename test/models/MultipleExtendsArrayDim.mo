// Two unnamed extends clauses where the second declares a dimension parameter
// used inside itself. The flattener must resolve the dimension in the correct
// extends scope, not the one cached from the first extends.

package P
  partial block Base1
    Real x;
    Real y;
  end Base1;

  partial block Base2
    parameter Integer n = 3;
    Real arr[n];
  end Base2;

  block Both
    extends Base1;
    extends Base2(n = 2);
  end Both;
end P;
