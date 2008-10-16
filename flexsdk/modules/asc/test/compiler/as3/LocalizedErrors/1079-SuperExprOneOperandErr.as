/*
1079	A super expression must have one operand.
*/

class A {
  function a():void {}
}

class B extends A {

}

var a:A = new A();
super().a;
