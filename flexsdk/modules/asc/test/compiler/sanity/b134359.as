package NestedGetterCrash {
	public class MyClass {
		var _x:int = 5;
		public function get outerX():int {
			//[ggrossman 08.03.05] nested accessors no longer permitted
			function get_x():int {
				return _x;
			}
			return get_x();
		}
		function set x(xx:int):void {
			_x = xx;
		}
	}
	
}

import NestedGetterCrash.*;

c = new MyClass();
trace(c.outerX);
