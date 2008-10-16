class A {
function m() { print("A.m") }
function n() { print(x) }
function o() { return "A.m" }
function p() { return x }
var x = 10
}
var a : A = new A
a.m()
a.n()
print(a.o())
print(a.p())
