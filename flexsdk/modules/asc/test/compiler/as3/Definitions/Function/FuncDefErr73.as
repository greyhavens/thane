package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr73 implements _If1Arg
 {
    public function fn(argB:String,argA:String):Number {};
 }
}
