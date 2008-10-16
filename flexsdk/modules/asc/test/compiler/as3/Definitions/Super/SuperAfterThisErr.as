// 8.6.1 Constructor Methods - restrictions on position of SuperStatement
package Errors {
	class SuperAfterThisErr {
		function SuperAfterThisErr() {
		}
	}
	
	class SuperAfterThisErr1 extends SuperAfterThisErr {
		private var x : String = ""
		function SuperAfterThisErr1() {
			this.x = "referenced"
			super() // illegal: after reference to this
		}
	}
	
	class SuperAfterThisErr2 extends SuperAfterThisErr {
		private var x : String = ""
		function SuperAfterThisErr2( b : Boolean ) {
			if ( b ) {
				this.x = "referenced"
			}
			super() // illegal: after (potential) reference to this
		}
	}
	
	class SuperAfterThisErr3 extends SuperAfterThisErr2 {
		private var b : Boolean = true
		function SuperAfterThisErr3() {
			super(b) // illegal: accessing non-local property
		}
	}
	
	class SuperAfterThisErr4 extends SuperAfterThisErr2 {
		private static var bStat : Boolean = true
		function SuperAfterThisErr4() {
			super(bStat) // illegal: accessing non-local property
		}
	}
}
