package Errors {
	class BaseClass {
		public function setString( n:Number ) : void {}
	}

	class OverrideDiffParamsErr extends BaseClass {
		override public function setString( n:Number, s:String ) : void {}
	}

}