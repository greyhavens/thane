package Errors {
	class BaseClass {
		virtual function foo () {}
	}

	class OverrideFnWODeclaringErr2 extends BaseClass  {
		virtual function foo() {}
	}

}