// OMC 1.24.4 allows redeclare to incompatible types as long as value (and other attributes?) are compatible
model M
    model BaseBool
        replaceable Boolean x = true;
    end BaseBool;
    model BaseReal
        replaceable Real x = 3.5; // Value not compatible with other types
    end BaseReal;
    model BaseInt
        replaceable Integer x = -1; // Value not compatible with Boolean or String
    end BaseInt;
    model BaseStr
        replaceable String x = "foo"; // Value not compatible with other types
    end BaseStr;

    model BoolToInt
        extends BaseBool(replaceable Integer x);  // Error: binding x = true
    end BoolToInt;
    model BoolToReal
        extends BaseBool(replaceable Real x);  // Error: binding x = true
    end BoolToReal;
    model BoolToStr
        extends BaseBool(replaceable String x);  // Error: binding x = true
    end BoolToStr;
    model IntToReal
        extends BaseInt(replaceable Real x);  // OK (binding x = -1)
    end IntToReal;
    model IntToStr
        extends BaseInt(replaceable String x);  // Error: binding x = -1
    end IntToStr;
    model RealToInt
        extends BaseReal(replaceable Integer x);  // Error: binding x = 3.5
    end RealToInt;
    model RealToBool
        extends BaseReal(replaceable Boolean x);  // Error: binding x = 3.5
    end RealToBool;
    model RealToStr
        extends BaseReal(replaceable String x);   // Error: binding x = 3.5
    end RealToStr;
    model ShortRealToBool = BaseReal(replaceable Boolean x);  // Error: binding x = 3.5
    model ShortRealToInt = BaseReal(replaceable Integer x);  // Error: binding x = 3.5
    model ShortStrToReal = BaseStr(replaceable Real x); // Error: binding x = "foo"
    model ShortStrToRealVal = BaseStr(replaceable Real x = 3.5); // OK (modified value is compatible)
    BoolToInt b2i;
    BoolToReal b2r;
    BoolToStr b2s;
    IntToReal i2r;
    IntToStr i2s;
    RealToInt r2i;
    RealToBool r2b;
    RealToStr r2s;
    ShortRealToBool sr2b;
    ShortRealToInt sr2i;
    ShortStrToReal ss2r;
    ShortStrToRealVal ss2rv;
end M;
