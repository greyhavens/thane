try
{
class A {
function A(x) { 
print("A$A",x) 
}
}
class B extends A
{
function B(y) { 
print("B$B",y) 
}
}
class C extends B
{
function C(z) { 
print("C$C",z) 
}
}
var a : A = new A(1)
var b : B = new B(2)
var c : C = new C(3)

class D {
function D(x) { print("D$D",x) }
}
class E extends D
{
function E(x,y) { 
super(x)
print("E$E",y) 
}
}
class F extends E
{
function F(x,y,z) { 
super(x,y)
print("F$F",z) 
}
}
var d : D = new D(1)
var e : E = new E(1,2)
var f : F = new F(1,2,3)
}
catch (e)
{
print("caught exception: "+e);
}
