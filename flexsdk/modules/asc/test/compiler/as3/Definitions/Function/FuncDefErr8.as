package Errors {

 interface _If2Arg
 {
    function fn(argA:Number,argB:String);
 }

 class FuncDefErr8 implements _If2Arg
 {
    public function fn(argB:String,argA:Number) {};
 }
}
