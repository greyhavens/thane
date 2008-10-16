/**
 *  	Class ExtDImplDDAccDMFPubStatErr
 *
 *  	Attempts to access a default method from
 *  	a public static method of the sub class
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

	class ExtDImplDDAccDMFPubStatErr extends DynamicClass implements DefaultIntDef, DefaultInt{

	    public static function iBaseClass() { return baseClass(); }


	}

	var out = new ExtDImplDDAccDMFPubStatErr();


	out.iBaseClass();

	// print( "FAILED, should give compile time error" );
}
