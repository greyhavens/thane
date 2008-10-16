/*
 *  Expected Verify Error
 * 
 *	
 */


package Errors {
	interface A {
		function a();
	}
	interface B {
		function b();
	}
	interface C extends A, B {}
	
	class D implements C {}
	
}