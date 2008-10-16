/*
 *  Expected Error
 * 
 *	Nested classes must be static
 */


package Errors {
	
	
	class MyClass {
		function foo() : int {}
		
		interface I { 
			function foo() : String
		}
	}
}