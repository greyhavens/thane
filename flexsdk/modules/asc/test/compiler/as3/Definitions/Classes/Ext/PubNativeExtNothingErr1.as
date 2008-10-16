/**
 *	Class PubNativeExtNothingErr1
 *	
 * 	Public native class is not allowed
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	public native class PubNativeExtNothingErr1 {

	     function foo() {
	      a = 20;
	     }

	}

	var b = new PubNativeExtNothingErr1();
	b.foo();

	// print( "FAILED, public native class should not be allowed" );
}
