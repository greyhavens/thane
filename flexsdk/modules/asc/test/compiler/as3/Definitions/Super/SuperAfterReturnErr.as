// 8.6.1 Constructor Methods - restrictions on position of SuperStatement
package Errors {
	class SuperAfterReturnErr {
		function SuperAfterReturnErr() {
		}
	}
	
	class SuperAfterReturnErr1 extends SuperAfterReturnErr {
		function SuperAfterReturnErr1() {
			return
			super() // illegal: after return
		}
	}
	
	class SuperAfterReturnErr2 extends SuperAfterReturnErr {
		function SuperAfterReturnErr2( b : Boolean ) {
			if ( b ) {
				return
			}
			super() // illegal: after (potential) return
		}
	}
}
