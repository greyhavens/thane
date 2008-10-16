///*

class A 
{
    prototype var x = 10
    prototype var f = function() { return prototype.x }
    
    //function bad() { prototype var wrong }
    prototype var g = function(a:int=1, b:String="foo") 
    {
          trace (a, b)
    }
}
//prototype var z

class B extends A
{
    prototype var x = 20
}

//*/
/*
class A {
prototype.x = 10
prototype._f = function () { return "A.f" }
function get f() { return prototype._f }
}
print(new A().x)
print(new A().f())
*/
/*

class A 
{
{
private namespace prototype_ns
prototype_ns var x
prototype_ns var x_is_set
public function get x() {
if( prototype_ns::x_is_set )
{
    return prototype_ns::x
}
else
{
    return prototype.x
}
}
public function set x(v) {
prototype_ns::x = v
prototype_ns::x_is_set = true
}
prototype.x = function() { return "A.x" }
}
}

*/

var a = new A
print(a.f()) // 10
var b = new B
print(b.x)   // 20
print(b.f()) // 10
a.x = 30
print(a.x)   // 30
print(b.x)   // 20 
b.x = 40
print(a.x)   // 30
print(b.x)   // 40
A.prototype.x = 50
print(a.x)   // 30
print(b.x)   // 40
print(b.f()) // 50
b.f = function() { return "b.f" }
print(b.f())

a.g()
a.g(10)
a.g(10, "string")

