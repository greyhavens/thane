package p {
public namespace N1
public class A {
N1 var x = 20
public namespace N2 = "foo"
}
}
import p.*
function f(ns=N1)
{
print(ns.uri)
print(new A().ns::x)
}
print(N1.uri)
print(A.N2)
f()
class B {
static A.N2 function g() { print("B.(A.N2)::g()") }
}
var ns = A.N2
B.ns::g()