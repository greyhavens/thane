package Errors {
	class BaseClass {
		public function setString( s:String ) : void {}
	}

	class OverrideDiffParamsErr extends BaseClass {
		override public function setString( s:Boolean ) : void {}
	}

}