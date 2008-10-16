package Errors {
	class BaseClass {
		private function foo(){}
	}

	class RedeclaringVarsErr extends BaseClass {
		override public var prop:String;
		override var foo:String;
		override private var bar:Number;
		override internal var b:Boolean;
	}

}