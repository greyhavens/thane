/**
 *  	Class ExtDynImplAccDMFPubStatErr
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


	class ExtDynImplAccessDMFPubStatErr extends DynamicClass implements DefaultIntDef {
	    public static function iBaseClass() { return baseClass(); }

	}

	var out = new ExtDynImplAccessDMFPubStatErr();

	out.iBaseClass();

	// print( "FAILED, should give compile time error" );
}
