// compiler should flag overrides with different
// access specifiers
package Errors {
	class base {
		function defFn() { }
		internal function intFn() { }
		public function pubFn() { }
		protected function protFn() { }
		private function privFn() { }
	}
	class defDerived extends base {
		override function defFn() { }	// legal
		override function intFn() { }	// legal
		override function pubFn() { }	// ILLEGAL 1 of 14
		override function protFn() { }	// ILLEGAL 2 of 14
		function privFn() { }	// legal - base version not visible, hence no override
	}
	class intDerived extends base {
		internal override function defFn() { }	// legal
		internal override function intFn() { }	// legal
		internal override function pubFn() { }	// ILLEGAL 3 of 14
		internal override function protFn() { }	// ILLEGAL 4 of 14
		internal function privFn() { }	// legal - base version not visible, hence no override
	}
	class pubDerived extends base {
		public override function defFn() { }	// ILLEGAL 5 of 14
		public override function intFn() { }	// ILLEGAL 6 of 14
		public override function pubFn() { }	// legal
		public override function protFn() { }	// ILLEGAL 7 of 14
		public function privFn() { }	// legal - base version not visible, hence no override
	}
	class protDerived extends base {
		protected override function defFn() { }	// ILLEGAL 8 of 14
		protected override function intFn() { }	// ILLEGAL 9 of 14
		protected override function pubFn() { }	// ILLEGAL 10 of 14
		protected override function protFn() { }	// legal
		protected function privFn() { }	// legal - base version not visible, hence no override
	}
	class privDerived extends base {
		private override function defFn() { }	// ILLEGAL 11 of 14
		private override function intFn() { }	// ILLEGAL 12 of 14
		private override function pubFn() { }	// ILLEGAL 13 of 14
		private override function protFn() { }	// ILLEGAL 14 of 14
		private function privFn() { }	// legal - base version not visible, hence no override
	}
}
