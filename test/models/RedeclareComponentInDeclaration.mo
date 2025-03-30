model M
    model BaseInt
        replaceable Integer x = 0;
    end BaseInt;
    model IntToReal1 = BaseInt(redeclare Real x = 1.0); // short extends redeclare
    model IntToReal2
        extends BaseInt(redeclare Real x = 2.0); // regular extends redeclare
    end IntToReal2;
    model IntToReal3 = BaseInt(replaceable Real x = 3.0); // short extends replaceable
    model IntToReal4
        extends BaseInt(replaceable Real x = 4.0); // regular extends replaceable
    end IntToReal4;
    model IntToReal5
        extends BaseInt(redeclare replaceable Real x = 5.0); // regular extends redeclare replaceable
    end IntToReal5;
    IntToReal1 ir1;
    IntToReal2 ir2;
    IntToReal3 ir3;
    IntToReal4 ir4;
    IntToReal5 ir5;
end M;
