/*
1054	Return type of a setter definition must be unspecified or void.
*/
class A
{
    private var _x
    function get x() { return _x }
    function set x(v):Number {
        _x = v;
        return _x;
    }
}
