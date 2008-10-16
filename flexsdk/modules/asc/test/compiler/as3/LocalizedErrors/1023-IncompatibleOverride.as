/* 
1023	Incompatible override.
*/
class A {
  function a():int {
    return 1;
  }
}

class B extends A {
  override function a():void {
  }
}