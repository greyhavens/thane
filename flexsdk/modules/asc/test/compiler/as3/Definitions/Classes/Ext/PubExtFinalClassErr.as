/**
 *	Class PubExtFinalClassErr
 *	
 *  	Public class attempts to inherit from a final class
 *
 *	Modified: 07/19/2005 (scorfield@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {
	
	namespace NS

	final class PubExtFinalClassErr {

	     	public var a = 10;
	}

	final internal class PubExtFinalInternalClassErr {

	     	public var a = 10;
	}

	final public class PubExtFinalPublicClassErr {

	     	public var a = 10;
	}

	final NS class PubExtFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic final class PubExtDynamicFinalClassErr {

	     	public var a = 10;
	}

	dynamic final internal class PubExtDynamicFinalInternalClassErr {

	     	public var a = 10;
	}

	dynamic final public class PubExtDynamicFinalPublicClassErr {

	     	public var a = 10;
	}

	dynamic final NS class PubExtDynamicFinalNSClassErr {

	     	public var a = 10;
	}

	public class B1 extends PubExtFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B2 extends PubExtFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B3 extends PubExtFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B4 extends NS::PubExtFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B5 extends PubExtDynamicFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B6 extends PubExtDynamicFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B7 extends PubExtDynamicFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	public class B8 extends NS::PubExtDynamicFinalNSClassErr {

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
