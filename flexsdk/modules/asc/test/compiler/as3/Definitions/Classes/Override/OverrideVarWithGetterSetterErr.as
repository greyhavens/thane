package GetterSetter {
	public class BaseClass {
		public var _string:String;
	}
	
	public class OverrideVarWithGetterSetter extends BaseClass {
		override public function get string() : String { return _string; }
		override public function set string( s:String ) { _string=s; }	
	}
}