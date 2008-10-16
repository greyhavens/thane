/**
 *	Class IntExtFinalClassErr
 *	
 *  	Internal class attempts to inherit from a final class
 *
 *	Modified: 07/19/2005 (scorfield@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {
	
	namespace NS

	final class IntExtFinalClassErr {

	     	public var a = 10;
	}

	final internal class IntExtFinalInternalClassErr {

	     	public var a = 10;
	}

	final public class IntExtFinalPublicClassErr {

	     	public var a = 10;
	}

	final NS class IntExtFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic final class IntExtDynamicFinalClassErr {

	     	public var a = 10;
	}

	dynamic final internal class IntExtDynamicFinalInternalClassErr {

	     	public var a = 10;
	}

	dynamic final public class IntExtDynamicFinalPublicClassErr {

	     	public var a = 10;
	}

	dynamic final NS class IntExtDynamicFinalNSClassErr {

	     	public var a = 10;
	}

	internal class B1 extends IntExtFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B2 extends IntExtFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B3 extends IntExtFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B4 extends NS::IntExtFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B5 extends IntExtDynamicFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B6 extends IntExtDynamicFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B7 extends IntExtDynamicFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	internal class B8 extends NS::IntExtDynamicFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	B1.foo();
	B2.foo();
	B3.foo();
	B4.foo();
	B5.foo();
	B6.foo();
	B7.foo();
	B8.foo();

	// print( "FAILED, should not be able to inherit from a final class" );
}
