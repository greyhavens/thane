package Errors {
	namespace ns1;
	class A {
		ns1 function foo() : String { return "foo" }
	}
	
	class B extends A {
		override function foo() : String { return "foo" }
	}

}