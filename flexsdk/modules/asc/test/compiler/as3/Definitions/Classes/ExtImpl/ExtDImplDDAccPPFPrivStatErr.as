/**
 *  	Class ExtDImplDDAccPPFPrivStatErr
 *
 *  	Attempts to access a public property from
 *  	a static method of the sub class
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

	class ExtDImplDDAccPPFPrivStatErr extends DynamicClass implements DefaultIntDef, DefaultInt {

	    private static function subGetArray() : Array { return pubArray; }

	    // interface definitions
	    function iGetBoolean() : Boolean {}
	    function iGetPubBoolean() : Boolean {}
	    function iGetNumber() : Number {}
	    function iGetPubNumber() : Number {}

	    function foo() { // print("Accessing foo()"); }
	    function goo() { // print("Accessing goo()"); }
	}

	var out = new ExtDImplDDAccPPFPrivStatErr();


	out.subGetArray();
	out.iGetBoolean();

	// print( "FAILED, should give compile time error for accessing the default property from a private static method" );
}
