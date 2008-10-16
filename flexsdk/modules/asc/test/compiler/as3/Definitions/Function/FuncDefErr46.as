package Errors {

 interface _If2Arg
 {
    function fn(argA:Number,argB:String);
 }

 class FuncDefErr46 implements _If2Arg
 {
    public function fn(argB:Number,argA:String):Number {};
 }
}
