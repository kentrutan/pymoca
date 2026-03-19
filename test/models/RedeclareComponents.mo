// Redeclare components of same type defined outside the model
package Package
    model Base
        replaceable Real x = 0;
    end Base;
    model Model
        Base b1(redeclare Integer x = 1),
             b2(redeclare Integer x = 2),
             b0;
    end Model;
end Package;;
