/*
 *  Expected Error
 * 
 *	Interface method foo in namespace Errors$internal  not implemented by class Errors$internal:MyClass
 */


package Errors {
	interface I { 
		function foo()
	}
	
	class MyClass implements I {
		public function foobar () {}
	}
}