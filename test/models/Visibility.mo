class A "A public class with multiple visibility declarations"
    protected
    class B
        parameter Real x, y, z;
    end B;
    B b;

    public
    class C
        protected
        extends B; // Protected extends in public class
    end C;
    C c, d;

    protected
    C e; // Protected component of a public type
    constant Real f;
end A;

