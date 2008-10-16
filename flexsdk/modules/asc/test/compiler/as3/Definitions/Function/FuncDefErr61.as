package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr61 implements _If1Arg
 {
    public function fn(argA:String,argB:String):Number {};
 }
}
