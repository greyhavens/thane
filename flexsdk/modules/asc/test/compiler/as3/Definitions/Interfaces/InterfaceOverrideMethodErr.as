/*
 *  Expected Error
 * 
 *	Method marked 'override' must override another method
 */


package Errors {
	interface A {
		function foo()
	}	

	interface B extends A { 
		override function foo()
	}
}