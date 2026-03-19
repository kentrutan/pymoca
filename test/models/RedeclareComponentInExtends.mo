model M
    model Base
        replaceable Real x = 0;
    end Base;
    model Derived1 = Base(redeclare Integer x = 1);
    model Derived2
        extends Base(redeclare Integer x = 2);
    end Derived2;
    model Derived3 = Base(replaceable Integer x = 3);
    model Derived4
        extends Base(replaceable Integer x = 4);
    end Derived4;
    model Derived5
        extends Base(redeclare replaceable String x = "5");
    end Derived5;
    Derived1 d1;
    Derived2 d2;
    Derived3 d3;
    Derived4 d4;
    Derived5 d5;
end M;
