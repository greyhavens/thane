class A {
var x = 10
function m() { return x }
}
class B extends A {
var y = 20
override function m() { return y }
final function n() { return y }
}
var a : A = new A
var b : B = new B
print(a.x)
print(b.y)