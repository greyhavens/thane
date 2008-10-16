/**
 *  	Class ExtDImplDDAccDMFPrivStatErr
 *
 *  	Attempts to access a default method from
 *  	a private static method of the sub class
 *
 *	Modified: 03/30/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	dynamic class DynamicClass {
		function baseClass() {
		 // print("base is invoked");
		}
	}

	interface DefaultIntDef {
	     function foo() {
	     }
	}

	interface DefaultInt {
	     function goo() {
	     }
	}

	class ExtDImplDDAccDMFPrivStatErr extends DynamicClass implements DefaultIntDef, DefaultInt {

	    private static function iBaseClass() : Array { return baseClass(); }

	}


	var out = new ExtDImplDDAccDMFPrivStatErr();


	out.iBaseClass();

	// print( "FAILED, should give compile time error" );
}
