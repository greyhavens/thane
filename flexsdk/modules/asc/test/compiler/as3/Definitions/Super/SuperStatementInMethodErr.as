// 14.3 Super Statement - only in a constructor
package Errors {
	class SuperStatementInMethodErr {
		function SuperStatementInMethodErr() {
		}
	}
	class SuperStatementInMethodErr1 extends SuperStatementInMethodErr {
		function SuperStatementInMethodErr1() {
			super()	// ok here
		}
		function f() {
			super()	// illegal in method
		}
		static function g() {
			super()	// illegal in static method
		}
		super()	// illegal in class init
	}
}
