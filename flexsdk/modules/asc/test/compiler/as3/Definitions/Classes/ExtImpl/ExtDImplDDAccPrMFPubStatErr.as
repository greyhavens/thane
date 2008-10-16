/**
 *  	Class ExtDImplDDAccPrMFPubStatErr
 *
 *  	Attempts to access a private method from
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

	class ExtDImplDDAccPrMFPubStatErr extends DynamicClass implements DefaultIntDef, DefaultInt {

	    public static function subGetArray() : Array { return getPrivArray(); }
	    public static function subSetArray(a:Array) { setPrivArray(a); }

	    // interface definitions
	    function iGetBoolean() : Boolean {}
	    function iGetPubBoolean () : Boolean {}
	    function iGetNumber() : Number {}
	    function iGetPubNumber() : Number {}

	    function foo() { // print("Accessing foo()"); }
	    function goo() { // print("Accessing goo()"); }
	}

	var out = new ExtDImplDDAccPrMFPubStatErr();


	out.subGetArray();
	out.iGetBoolean();

	// print( "FAILED, should give compile time error for accessing the default property from a private static method" );
}
