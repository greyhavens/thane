/* 
1025	Cannot redefine a final method.
*/
class A {
  final function a():void {
  }
}

class B extends A {
  override function a():void {
  }
}