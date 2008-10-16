namespace v1
use namespace v1
class A {
    static v1 function f() { 
        print("A.v1::f",v1::x,x)
    }
    static v1 var x = 10
}
A.v1::f()
A.f()
print(A.x,A.v1::x)
