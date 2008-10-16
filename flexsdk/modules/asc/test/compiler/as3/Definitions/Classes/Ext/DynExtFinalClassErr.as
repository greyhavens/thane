/**
 *	Class DynExtFinalClassErr
 *	
 *  	Dynamic class attempts to inherit from a final class
 *
 *	Modified: 07/19/2005 (scorfield@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {
	
	namespace NS

	final class DynExtFinalClassErr {

	     	public var a = 10;
	}

	final internal class DynExtFinalInternalClassErr {

	     	public var a = 10;
	}

	final public class DynExtFinalPublicClassErr {

	     	public var a = 10;
	}

	final NS class DynExtFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic final class DynExtDynamicFinalClassErr {

	     	public var a = 10;
	}

	dynamic final internal class DynExtDynamicFinalInternalClassErr {

	     	public var a = 10;
	}

	dynamic final public class DynExtDynamicFinalPublicClassErr {

	     	public var a = 10;
	}

	dynamic final NS class DynExtDynamicFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic class B1 extends DynExtFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B2 extends DynExtFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B3 extends DynExtFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B4 extends NS::DynExtFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B5 extends DynExtDynamicFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B6 extends DynExtDynamicFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B7 extends DynExtDynamicFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	dynamic class B8 extends NS::DynExtDynamicFinalNSClassErr {

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
