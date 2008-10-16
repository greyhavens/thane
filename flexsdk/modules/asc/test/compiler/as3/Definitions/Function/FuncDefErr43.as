package Errors {

 interface _If2Arg
 {
    function fn(argA:Number,argB:String);
 }

 class FuncDefErr43 implements _If2Arg
 {
    public function fn(argB:Number,argA:Number):Number {};
 }
}
