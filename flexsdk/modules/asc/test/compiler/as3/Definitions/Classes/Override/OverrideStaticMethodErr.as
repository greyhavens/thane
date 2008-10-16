package Errors {
	class BaseClass {
		public static function foo(){}
	}

	class OverrideParentFinalMethodErr extends BaseClass {
		override public static function foo() {}
	}

}