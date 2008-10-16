/* 
1028	Functions cannot be both static and virtual.
*/
class A {
    static virtual function a():void {}
}
