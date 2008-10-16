class A {
var y
private var _z
function A(x) {
print(x==10)
}
function get z() { return _z }
function set z(v) { _z = v }
}
class B extends A {
function B(x) {
super(x)
super.y = 1
print(super.y)
z = 2
print(z)
}
override function get z() { return super.z }
override function set z(v) { super.z = v }
}
new B(10)
