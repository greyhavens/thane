package Errors {
	class BaseClass {
		public var prop:String;
		var foo:String;
		private var bar:Number;
		internal var b:Boolean;

		private function foo(){}
	}

	class RedeclaringVarsErr extends BaseClass {
		override public var prop:String;
		override var foo:String;
		override private var bar:Number;
		override internal var b:Boolean;
	}

}