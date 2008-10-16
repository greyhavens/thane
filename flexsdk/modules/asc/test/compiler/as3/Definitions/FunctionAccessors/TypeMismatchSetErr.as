package Errors {

 interface _If1Arg
 {
    function set fn(argA:String):void;
 }

 class FuncDefErr0 implements _If1Arg
 {
    var n:Number;
    public function set fn(argA:Number) { n = argA; }
 }

}
