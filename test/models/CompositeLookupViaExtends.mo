// Tests composite name lookup (e.g. clutch.tau) where the attribute lives
// in an extends-of-extends chain of the component's type class.

package P
  partial model PartialCompliant
    Real tau;
  end PartialCompliant;

  partial model PartialTwoFlanges
    Real flange_a;
    Real flange_b;
  end PartialTwoFlanges;

  // tau is two extends levels away for Clutch:
  //   Clutch -> PartialCompliantWithRelativeStates -> PartialCompliant -> tau
  partial model PartialCompliantWithRelativeStates
    extends PartialTwoFlanges;
    extends PartialCompliant;
    Real phi_rel;
  end PartialCompliantWithRelativeStates;

  model Clutch
    extends PartialCompliantWithRelativeStates;
    Real fn;
  end Clutch;
end P;

model CompositeLookupViaExtends
  P.Clutch clutch;
  // Resolving clutch.tau requires traversing Clutch's extends chain
  Real tau_copy = clutch.tau;
end CompositeLookupViaExtends;
