/**
 *	Class NativeExtPubClassErr1
 *	
 *  	Accesses default property of public class
 *  	from final method of a native sub class
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	public class NativeExtPubClassErr1 {
		var a = 10;
	}

	native class B extends NativeExtPubClassErr1 {

	     final function foo() {
	      a = 20;
	     }

	}

	var b = new B();
	b.foo();

	// print( "FAILED, native class should not be allowed" );
}
