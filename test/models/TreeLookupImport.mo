within Level1.Level2.Level3;

package TestPackage
	class TestClass
		Integer i;
		Integer a;
	end TestClass;
	class TestClass2
		Real i;
		Real a;
	end TestClass2;
end TestPackage;

package TestPackage2
	class TestClass
		Boolean b;
	end TestClass;
end TestPackage2;

model PackageComponents
	Level2.Level3.TestPackage.TestClass tc;
equation
	tc.i = 1;
end PackageComponents;

model Test
    import Type.*;
	// import TPTCi = TestPackage.TestClass.i; // Error: In "import A.B.C", A.B must be a package
	import TestPackage.{TestClass,TestClass2};
	// import TestPackage2;
	TestClass2 tc2;
	Integer a=10;
	Real z=0.001526;
	Real q=c+z;
	Coefficient c = 3.14;
	// Real my_i = TPTCi; // Type conversion
	Level1.Level2.Level3.PackageComponents elem(tc.a=a*2);
	// Integer b = 3;
	TestClassExtended tce;
	model TestClassExtended
	  extends TestClass;
	  import TestPackage2.TestClass;
	  TestClass pkg2tc;
	equation
	  pkg2tc.b = false;
	end TestClassExtended;
equation
// 	elem.tc.a = b;
	tc2.i = 1.0;
	tc2.a = 2.0;
end Test;
