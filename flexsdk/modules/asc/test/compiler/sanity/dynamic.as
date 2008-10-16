dynamic class A { 
}
class B extends A {
}
var a : A = new A
a.x = 10  // okay
print(a.x)
var b : B = new B
b.x = 20  // runtime error
print(b.x)
