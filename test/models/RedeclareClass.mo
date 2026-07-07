// Redeclare class of contained component defined outside the model
package Package
    model Base
        replaceable type BoolType = Boolean;
        BoolType b = false;
    end Base;
    model Model
        type IntType = Integer;
        type BoolType = Real; // Should not be used
        // b1 modification lhs scope is b1, rhs scope is Model
        Base b0, b1(redeclare type BoolType=IntType, b=3), b2;
    end Model;
end Package;
