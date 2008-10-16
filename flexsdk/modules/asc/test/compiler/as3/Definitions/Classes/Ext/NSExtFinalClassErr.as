/**
 *	Class NSExtFinalClassErr
 *	
 *  	Namespace class attempts to inherit from a final class
 *
 *	Modified: 07/19/2005 (scorfield@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {
	
	namespace NS

	final class NSExtFinalClassErr {

	     	public var a = 10;
	}

	final internal class NSExtFinalInternalClassErr {

	     	public var a = 10;
	}

	final public class NSExtFinalPublicClassErr {

	     	public var a = 10;
	}

	final NS class NSExtFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic final class NSExtDynamicFinalClassErr {

	     	public var a = 10;
	}

	dynamic final internal class NSExtDynamicFinalInternalClassErr {

	     	public var a = 10;
	}

	dynamic final public class NSExtDynamicFinalPublicClassErr {

	     	public var a = 10;
	}

	dynamic final NS class NSExtDynamicFinalNSClassErr {

	     	public var a = 10;
	}

	NS class B1 extends NSExtFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B2 extends NSExtFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B3 extends NSExtFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B4 extends NS::NSExtFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B5 extends NSExtDynamicFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B6 extends NSExtDynamicFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B7 extends NSExtDynamicFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	NS class B8 extends NS::NSExtDynamicFinalNSClassErr {

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
