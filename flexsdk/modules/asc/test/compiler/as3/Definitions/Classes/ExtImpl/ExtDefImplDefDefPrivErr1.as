/**
 *  	Class ExtDefImplDefPrivErr1
 *
 *
 *  	Attempts to access a default property from
 *  	a private method of a default class
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


	class ExtDefImplDefDefPrivErr1 extends DefaultClass implements DefaultIntDef, DefaultInt {


	     static function foo() { // print("Accessing foo()"); }
	     static function goo() { // print("Accessing goo()"); }
	}

	var out = new ExtDefImplDefDefPrivStatErr1();


	out.iBaseClass();

	// print( "FAILED, should give compile time error as sub class is  static" );
}
