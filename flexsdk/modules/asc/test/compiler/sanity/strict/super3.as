class A {
var x
function A() { 
x = 10
print('A')
}
function m() { print(x) }
}
class B extends A {
function B() { 
super()
x = 20
print('B')
}
override function m() { super.m() }
function n(o) { super(o).m() }
}
var a = new A
var b = new B
b.m()
b.n(a)
b.n(b)

