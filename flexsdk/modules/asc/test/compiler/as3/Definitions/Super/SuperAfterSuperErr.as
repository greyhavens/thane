// 8.6.1 Constructor Methods - restrictions on position of SuperStatement
package Errors {
	class SuperAfterSuperErr {
		function SuperAfterSuperErr() {
		}
		function f() {
		}
	}
	
	class SuperAfterSuperErr1 extends SuperAfterSuperErr {
		function SuperAfterSuperErr1() {
			super.f()
			super() // illegal: after reference to super
		}
	}
	
	class SuperAfterSuperErr2 extends SuperAfterSuperErr {
		function SuperAfterSuperErr2( b : Boolean ) {
			if ( b ) {
				super.f()
			}
			super() // illegal: after (potential) reference to super
		}
	}
}
