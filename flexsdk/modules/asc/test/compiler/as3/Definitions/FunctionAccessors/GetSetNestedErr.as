package NestedGetterCrash {
public class MyClass {
var _x:int = 5;
public function get outerX():int {
function get x():int {
return _x;
}
return x;
}
function set x(xx:int):void {
_x = xx;
}
}

}

import NestedGetterCrash.*;

c = new MyClass();
trace(c.outerX);
