
/*
 * Expected Error
 *
 * Duplicate function definition
 */
package Errors {
	class A {
		function foo(){}
	}
	
	class B extends A {
		override function foo(){}
		override function foo(){}
	}

}