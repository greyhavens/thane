class A {
static var cx = "A.cx"
var ix = "A.ix"
static function cm() { return "A.cm" }
function A() { }
function im() { return "A.im" }
function ir() { return "A.ir" }
function iq() { return "A.iq" }
}
class B extends A
{
static var cy = "B.cy"
var iy = "B.iy"
static function cn() { return "B.cn" }
function B() { }
override function im() { return "B.im" }
function io() { return "B.io" }
}
class C extends B
{
static var cz = "C.cz"
var iz = "C.iz"
function C () {}
static function co() { return "C.co" }
override function im() { return "C.im" }
function ip() { return "C.ip" }
}

print("A.cx"==A.cx)
//print("A.cx"==B.cx)
print("B.cy"==B.cy)
//print("A.cx"==C.cx)
//print("B.cy"==C.cy)
print("C.cz"==C.cz)
print("A.cm"==A.cm())
//print("A.cm"==B.cm())
print("B.cn"==B.cn())
//print("A.cm"==C.cm())
//print("B.cn"==C.cn())
print("C.co"==C.co())

var a : A = new A
var b : B = new B
var c : C = new C

print("A.im"==a.im())
print("A.ir"==a.ir())
print("A.iq"==a.iq())
print("B.im"==b.im())
print("A.ir"==b.ir())
print("A.iq"==b.iq())
print("B.io"==b.io())
print("C.im"==c.im())
print("C.ip"==c.ip())
