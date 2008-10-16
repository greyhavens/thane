package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr58 implements _If1Arg
 {
    public function fn(argA:Number,argB:String):Number {};
 }
}
