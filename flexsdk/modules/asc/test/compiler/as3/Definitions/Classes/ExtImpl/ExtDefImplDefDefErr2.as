/**
 *  	Class ExtDefImplDefDefErr2
 *
 *  	Does not define public method from the interfaces
 *
 *	Modified: 03/30/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	class DefaultClass {
		function baseClass() {
		 // print("base is invoked");
		}
	}

	interface DefaultIntDef {
	     function foo() {
	     }
	}

	interface DefaultInt {
	     public function goo() {
	     }
	}

	class ExtDefImplDefDefErr extends DefaultClass implements DefaultIntDef, DefaultInt {


	    function iGetPubNumber() : Number { return 1; }


	}

	var out = new ExtDefImplDefDefErr1();


	out.iBaseClass();

	// print( "FAILED, should give compile time error for not implementing the interface" );
}
