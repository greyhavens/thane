class A { }
class B extends A { }

function f(a:A):B
{
   return a
}

function test() {
var a:A = new A
try {
f(a)
print("fail")
} catch (e) {
print("pass")
}
}

test()
