/**
 *	Class FinalExtFinalClassErr
 *	
 *  	Final class attempts to inherit from a final class
 *
 *	Modified: 07/19/2005 (scorfield@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {
	
	namespace NS

	final class FinalExtFinalClassErr {

	     	public var a = 10;
	}

	final internal class FinalExtFinalInternalClassErr {

	     	public var a = 10;
	}

	final public class FinalExtFinalPublicClassErr {

	     	public var a = 10;
	}

	final NS class FinalExtFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic final class FinalExtDynamicFinalClassErr {

	     	public var a = 10;
	}

	dynamic final internal class FinalExtDynamicFinalInternalClassErr {

	     	public var a = 10;
	}

	dynamic final public class FinalExtDynamicFinalPublicClassErr {

	     	public var a = 10;
	}

	dynamic final NS class FinalExtDynamicFinalNSClassErr {

	     	public var a = 10;
	}

	final class B1 extends FinalExtFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B2 extends FinalExtFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B3 extends FinalExtFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B4 extends NS::FinalExtFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B5 extends FinalExtDynamicFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B6 extends FinalExtDynamicFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B7 extends FinalExtDynamicFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	final class B8 extends NS::FinalExtDynamicFinalNSClassErr {

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
