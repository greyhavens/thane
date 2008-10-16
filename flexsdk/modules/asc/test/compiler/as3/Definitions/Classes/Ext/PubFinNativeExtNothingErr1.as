/**	
 *	Class PubFinNativeExtNothingErr1
 *	
 * 	Public final native class is not allowed
 *  
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	public final native class PubFinNativeExtNothingErr1 {

	     function foo() {
	      a = 20;
	     }

	}

	var b = new PubFinNativeExtNothingErr1();
	b.foo();

	// print( "FAILED, public final native class should not be allowed" );
}
