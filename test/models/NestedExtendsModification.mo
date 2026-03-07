package NestedExtendsModification
"Three levels of extends with value modifications to test modification ordering"

    model ValC
        encapsulated model ValD
            encapsulated model ValF
                Real x = 1.0;
            end ValF;
            extends ValF(x = 2.0);  // Level 1: value override
        end ValD;
    end ValC;

    model ValA
        extends ValC(ValD(x = 3.0));  // Level 2: value override
    end ValA;

    model ValM
        extends ValA.ValD(x = 4.0);  // Level 3: value override — should win
    end ValM;

end NestedExtendsModification;
