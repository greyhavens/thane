package Errors {
	class BaseClass {
		public function returnString() : int {}
	}

	class OverrideParentFinalMethodErr extends BaseClass {
		override public function returnString() : Boolean {}
	}

}