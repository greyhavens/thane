class A {
function get x() { print("get x",_x); return _x }
function set x(p) { _x = p; print("set x",p) }
var _x
static function get y() { print("get y",_y); return _y }
static function set y(p) { _y = p; print("set y",p) }
static var _y
}
function get z() { print("get z",_z); return _z }
function set z(p) { _z = p; print("set z",p) }
var _z
/*
[ggrossman 08/03/05] Nested accessors no longer permitted
function f() {
function get x() { print("get x",_x); return _x }
function set x(p) { _x = p; print("set x",p) }
var _x = 0
x = 10
print(x==10)
}

f()*/
var a : A = new A
a.x = 20
print(a.x == 20)
A.y = 30
print(A.y == 30)
z = 40
print(z == 40)

