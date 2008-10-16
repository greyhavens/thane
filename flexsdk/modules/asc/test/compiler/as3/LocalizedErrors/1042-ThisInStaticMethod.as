/*
1042	The this keyword can not be used in static methods. It can only be used in instance methods, function closures, and global code.
*/
class A {
    static var a:Number;
    static function b():void {
        this.a = 0;
    }
}