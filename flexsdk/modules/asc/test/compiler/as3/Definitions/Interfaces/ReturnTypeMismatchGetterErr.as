/*
 *
 *
 */
package Errors {
	interface I {
		function get n() : Number
	}
	
	class MyClass implements I {
		var _n : String;
		public function get n() : String {
			return _n;
		}
	}
}
