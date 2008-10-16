class A {
function f() { return "f in A" }
function g() { return "g in A" }
function dumpA() {
	print("A.x =",x);
	print("A.y =",this.y)
}
function A() { print("A") }
var x = 100
}
class B extends A {
function B() { print("B") }
override function f() { return "f in B" }
function dumpB() { print("B =",x,y) }
var y = 200
}
class C extends B {
function C() { print("C") }
override function g() { return "g in C" }
function dumpC() { print("C =",x,y,z) }
var z = 300
}

var o;

try {
o = new A
print(o.f())
print(o.g())
o.dumpA()
} catch (e) {}

o = new B
print(o.f())
print(o.g())

o = new C
print(o.f())
print(o.g())
print(o.x)
print(o.y)
print(o.z)
o.x = 10
o.y = 20
o.z = 30
print(o.x)
print(o.y)
print(o.z)
o.dumpA()
o.dumpB()
o.dumpC()
