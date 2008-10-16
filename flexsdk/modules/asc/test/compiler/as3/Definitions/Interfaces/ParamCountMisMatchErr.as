/*
 *  Expected Error
 * 
 *	Interface method foo in namespace Errors$internal  implemented with 
 *  incompatible signature in class Errors$internal:MyClass
 */


package Errors {
	interface I { 
		function foo(s:String)
	}
	
	class MyClass implements I {
		public function foo (s:String, i:int) {}
	}
}