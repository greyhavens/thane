package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr13 implements _If1Arg
 {
    public function fn(argB:Number,argA:Number) {};
 }
}
