package p {
class A {
protected var x = 10
}
class B extends A {
protected var y = 20
}
class C extends B {

protected var z = 30
}
public class D extends C {
public function f() {
print(x,y,z)
}
}
}
import p.*
var d : D = new D
d.f()