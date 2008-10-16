package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr70 implements _If1Arg
 {
    public function fn(argB:String,argA:Number):Number {};
 }
}
