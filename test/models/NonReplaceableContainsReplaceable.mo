package P

  model Outer
      parameter Real a = 1.0;
      replaceable encapsulated model InnerModel
          parameter Real b = 2.0;
      end InnerModel;

      InnerModel innerInstance;  // This is NOT replaceable
  end Outer;

  model CustomModel
    parameter Integer b = 3;
  end CustomModel;

  model CustomModel2
    extends Outer.InnerModel(redeclare parameter Integer b = 4);
  end CustomModel2;

  model Test
      Outer o(redeclare model InnerModel = CustomModel);
  end Test;

  model TestFail
      "This should fail because CustomModel2 should be transitively non-replaceable"
      Outer o(redeclare model InnerModel = CustomModel2);
  end TestFail;

end P;
