package p {
public namespace ns
public class A
{
ns function f() { print("A.f") }
}
}
import p.*
class B extends A 
{
override ns function f() { print("B.f") }
}
use namespace ns
new B().f()
