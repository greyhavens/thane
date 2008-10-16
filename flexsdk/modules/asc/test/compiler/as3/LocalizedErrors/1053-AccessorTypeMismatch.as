/*
1053	Accessor types must match.
*/
class A
{
    private var _x;
    function get x():Boolean { return _x; }
    function set x(v:String) { _x = v; }
}
