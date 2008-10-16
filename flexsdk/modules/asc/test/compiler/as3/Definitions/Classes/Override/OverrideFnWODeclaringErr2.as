package Errors {
	class BaseClass {
		public function foo () {}
	}

	class OverrideFnWODeclaringErr2 extends BaseClass  {
		public function foo() {}
	}

}