// Redeclare a replaceable symbol using a qualified (dotted) type name.
// After flattening P.User, src.offset must carry value 1.0.

package P
  partial block Base
    parameter Real offset = 0;
  end Base;

  package Inner
    block Concrete
      extends P.Base;
    end Concrete;
  end Inner;

  block Container
    replaceable Base src(offset = 1.0);
  end Container;

  block User
    extends Container(redeclare Inner.Concrete src);
  end User;
end P;
