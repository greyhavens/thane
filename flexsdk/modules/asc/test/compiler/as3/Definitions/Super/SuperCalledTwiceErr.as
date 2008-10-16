// 8.6.1 Constructor Methods - restrictions on count of SuperStatement
// 16.3.4 Class definition errors - constructor that calls super more than once
package Errors {
	class SuperCalledTwiceErr {
		function SuperCalledTwiceErr() {
		}
	}
	class SuperCalledTwiceErr1 extends SuperCalledTwiceErr {
		function SuperCalledTwiceErr1() {
			super()
			super() // second call is syntax error
		}
	}
	class SuperCalledTwiceErr2 extends SuperCalledTwiceErr {
		function SuperCalledTwiceErr2( b : Boolean ) {
			if ( b ) {
				super()
			}
			super() // (potential) second call is syntax error
		}
	}
}
