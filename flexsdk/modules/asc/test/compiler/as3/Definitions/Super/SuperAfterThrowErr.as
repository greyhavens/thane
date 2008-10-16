// 8.6.1 Constructor Methods - restrictions on position of SuperStatement
package Errors {
	class SuperAfterThrowErr {
		function SuperAfterThrowErr() {
		}
	}
	
	class SuperAfterThrowErr1 extends SuperAfterThrowErr {
		function SuperAfterThrowErr1() {
			throw "thrown"
			super() // illegal: after throw
		}
	}
	
	class SuperAfterThrowErr2 extends SuperAfterThrowErr {
		function SuperAfterThrowErr2( b : Boolean ) {
			if ( b ) {
				throw "thrown"
			}
			super() // illegal: after (potential) throw
		}
	}
}
