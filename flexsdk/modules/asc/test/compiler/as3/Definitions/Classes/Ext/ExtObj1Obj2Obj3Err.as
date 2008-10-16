/**
 *	Class ExtObj1Obj2Err
 *	
 *  	Attempts to inherit from three class objects.
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	class A {
	 	var a;
	}

	class B {
	 	function foo() { 
			// print("ClassB");
		}
	}

	class C {
	 	var c;
	}

	var obj1 = new A();
	var obj2 = new B();
	var obj3 = new C();

	class ExtObj1Obj2Obj3Err extends obj1,obj2,obj3 {}
}
