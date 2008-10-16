package Errors {
	class BaseClass {
		final public function foo(){}
	}

	class OverrideParentFinalMethodErr extends BaseClass {
		override public function foo() {}
	}

}