package Errors {

	internal class A{
		var a:Object;
	}	
	public class overrideVarWithGetter extends A {
		private var _x:Object;
		override function get a():Object{
			return _x;
		}	
	}

}
