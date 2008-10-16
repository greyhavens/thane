package Errors {
	class BaseClass {
		public function foo(){}
	}

	class RedeclaringVarsErr extends BaseClass {

		override public var foo:String;
	}

}