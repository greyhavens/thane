package Errors {
	class BaseClass {
		protected function foo () {}
	}

	class OverrideFnWODeclaringErr2 extends BaseClass  {
		protected function foo() {}
	}

}