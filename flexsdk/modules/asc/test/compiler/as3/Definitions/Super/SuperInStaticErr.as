// 8.6.2 Static Methods - super cannot appear here
package Errors {
	class SuperInStaticErr {
		function f() {
		}
	}
	class SuperInStaticErr1 {
		static function g() {
			super.f()	// illegal in a static method
		}
	}
}
