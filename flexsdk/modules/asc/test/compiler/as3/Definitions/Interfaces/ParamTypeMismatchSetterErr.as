/*
 *
 *
 */
package Errors {
	interface I {
		function set n(num:Number) : void
	}
	
	class MyClass implements I {
		var _n : String;
		public function set n(num:String) : void {
			_n = num;
		}
	}
}
