package Modelica
  package Mechanics
    package Rotational
      package Interfaces
        partial model PartialFriction
          constant Integer Backward = -1;
          constant Integer Forward = 1;
          constant Integer Unknown = 3;
          Integer mode(final min = Backward, final max = Unknown, start = Unknown, fixed = true);
          Real sa;
        end PartialFriction;

        partial model PartialElementaryTwoFlanges
          Real flange_a;
          Real flange_b;
        end PartialElementaryTwoFlanges;
      end Interfaces;

      package Components
        model BearingFriction
          extends Rotational.Interfaces.PartialElementaryTwoFlanges;
          extends Rotational.Interfaces.PartialFriction;
          Real phi;
          Real w;
        end BearingFriction;
      end Components;
    end Rotational;
  end Mechanics;
end Modelica;

model GearType1
  Modelica.Mechanics.Rotational.Components.BearingFriction bearingFriction;
end GearType1;
