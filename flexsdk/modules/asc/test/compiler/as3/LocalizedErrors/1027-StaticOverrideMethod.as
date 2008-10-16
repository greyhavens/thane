/* 
1027	Functions cannot be both static and override.
*/
class A {
    function a():void {}
}

class B extends A {
    static override function a():void {}
}
