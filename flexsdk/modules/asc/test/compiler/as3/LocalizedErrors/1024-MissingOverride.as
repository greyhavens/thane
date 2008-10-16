/* 
1024	Overriding a function that is not marked for override.
*/
class A {
  function a():void {
  }
}

class B extends A {
  function a():void {
  }
}