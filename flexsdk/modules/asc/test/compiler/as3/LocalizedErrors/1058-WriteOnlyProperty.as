/*
1058	Property is write-only.
*/
class A
{
    private var _x;
    //function get x() { return _x; }
    function set x(v) { _x = v; }
}

function f():void {
    var a:A = new A();
    a.x;
}
