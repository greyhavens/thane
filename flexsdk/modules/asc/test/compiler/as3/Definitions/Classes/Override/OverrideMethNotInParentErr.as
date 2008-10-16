package Errors {
	class BaseClass {
		public function foo(){}
	}

	class OverrideParentFinalMethodErr extends BaseClass {
		override public function pubFunction() {}
		override virtual function virtFunction() {}
		override private function privFunction() {}
	}

}