/**
 *	Class ExtFinalClassErr
 *	
 *  	Default class attempts to inherit from a final class
 *
 *	Modified: 07/19/2005 (scorfield@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {
	
	namespace NS

	final class ExtFinalClassErr {

	     	public var a = 10;
	}

	final internal class ExtFinalInternalClassErr {

	     	public var a = 10;
	}

	final public class ExtFinalPublicClassErr {

	     	public var a = 10;
	}

	final NS class ExtFinalNSClassErr {

	     	public var a = 10;
	}

	dynamic final class ExtDynamicFinalClassErr {

	     	public var a = 10;
	}

	dynamic final internal class ExtDynamicFinalInternalClassErr {

	     	public var a = 10;
	}

	dynamic final public class ExtDynamicFinalPublicClassErr {

	     	public var a = 10;
	}

	dynamic final NS class ExtDynamicFinalNSClassErr {

	     	public var a = 10;
	}

	class B1 extends ExtFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B2 extends ExtFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B3 extends ExtFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B4 extends NS::ExtFinalNSClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B5 extends ExtDynamicFinalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B6 extends ExtDynamicFinalInternalClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B7 extends ExtDynamicFinalPublicClassErr {

	     	public function foo() {
	      		a = 20;
	      		// print(a);
	     	}

	}

	class B8 extends NS::ExtDynamicFinalNSClassErr {

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
