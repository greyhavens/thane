/**
 *  	Class ExtDynImplAccessDMFPrivStatErr
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


	class ExtDynImplAccessDMFPrivStatErr extends DynamicClass implements DefaultIntDef {
	    private static function iBaseClass() { return baseClass(); }

	}

	var out = new ExtDynImplAccessDMFPrivStatErr();

	out.iBaseClass();

	// print( "FAILED, should give compile time error" );
}
