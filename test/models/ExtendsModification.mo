connector HQPort
  Real H;
  flow Real Q;
end HQPort;

partial model HQOnePort
  HQPort HQ;
end HQOnePort;

partial class Volume
  Real V(min = 0, nominal = 1e6);
end Volume;

partial model PartialStorage
  extends HQOnePort;
  extends Volume;
equation
  der(V) = HQ.Q;
end PartialStorage;

model Linear
  extends PartialStorage(HQ.H(min = H_b));
  parameter Real A;
  parameter Real H_b;
equation
  V = A * (HQ.H - H_b);
end Linear;

model MainModel
	Linear e(H_b=-2.0, A=1000.0);
equation
end MainModel;

// LinearExpr: same as Linear but the cross-scope modification value is an
// Expression (2 * H_b) rather than a bare ComponentRef.  This exercises the
// expression-operand path of _resolve_modification_attribute with a
// base-class scope (PartialStorage) that differs from the flat-naming root
// (MainModelExpr).
model LinearExpr
  extends PartialStorage(HQ.H(min = 2 * H_b));
  parameter Real A;
  parameter Real H_b;
equation
  V = A * (HQ.H - H_b);
end LinearExpr;

model MainModelExpr
  LinearExpr e(H_b=-2.0, A=1000.0);
equation
end MainModelExpr;
