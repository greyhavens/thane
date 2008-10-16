/*
1118	Implicit coercion of a value with static type _ to a possibly unrelated type _.
*/
class A {
    public function A() { super() }
}

class B extends A {
    public function B() { super() }
    public function f():void {}
}

var a:A = new A();
var b : B = a // error
