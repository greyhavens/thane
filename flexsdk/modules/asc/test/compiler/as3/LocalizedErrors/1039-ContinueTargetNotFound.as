/*
1039	Target of continue statement was not found.
*/
class A {
    function a():void {
        while (true) {
            continue test;
        }
    }
}