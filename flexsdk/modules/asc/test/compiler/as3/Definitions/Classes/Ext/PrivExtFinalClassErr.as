/**
 *	Class PrivExtFinalClassErr
 *	
 *  	Private class attempts to inherit from a final class
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	class outer {

	 	final class PrivExtFinalClassErr {

	     		function foo() {
	       			a = 20;
	     		}
	 	}

	 	private class B extends PrivExtFinalClassErr {
			private var a = 10;
	 	}
	}

	// print( "FAILED, should not be able to extend a final class" );
}
