// After flattening P.User, src.offset must still carry value 1.0.

package P
  partial block Base
    parameter Real offset = 0;
  end Base;

  block Concrete
    extends Base;
  end Concrete;

  block Container
    replaceable Base src(offset = 1.0);
  end Container;

  block User
    extends Container(redeclare Concrete src); // gets offset from extends Base
  end User;
end P;
