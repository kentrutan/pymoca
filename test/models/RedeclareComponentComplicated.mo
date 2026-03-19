package P
"Combinations of redeclaring components in a complicated model hierarchy
including indirectly redeclared components and encapsulated models"
    model A
        // Doesn't have a class D itself, but gets it via C
        extends C(D(replaceable Integer num=4, redeclare E b(num=5)));
    end A;
    model C
        encapsulated model D
            extends F(redeclare replaceable G num=3); // Component redeclare in extends
            encapsulated type G = Real;
            model B
                extends F(replaceable Real num=2); // Component redeclare in extends
            end B;
            encapsulated model F
                replaceable Integer num = 1;
            end F;
            replaceable B b;
        end D;
        D d;
    end C;
    model E
        replaceable Integer num = 0;
    end E;
    model M
        extends A.D; // Modified indirectly via A
        /* Component redeclare not in extends followed by no redeclare in same declaration.
         * Components themselves are replaceable.
         */
        replaceable C.D.F f(redeclare Real num=6), f2;
    end M;
    model M2
        extends A.D(redeclare Real num = 7.0); // Redeclare already redeclared component
        A.D.G g = 8.0; // No redeclare, not replaceable
    end M2;
end P;
