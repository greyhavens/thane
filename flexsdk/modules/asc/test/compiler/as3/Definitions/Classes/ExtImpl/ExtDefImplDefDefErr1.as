/**
 *  	Class ExtDefImplDefDefErr1
 *
 *  	Does not define default method from the interfaces
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
	     	function goo() {
	     	}
	}

	class ExtDefImplDefDefErr1 extends DefaultClass implements DefaultIntDef, DefaultInt {

	    	function iBaseClass() { baseClass(); }

	}

	var out = new ExtDefImplDefDefErr1();

	out.iBaseClass();

	// print( "FAILED, should give compile time error for not implementing the interface" );
}
