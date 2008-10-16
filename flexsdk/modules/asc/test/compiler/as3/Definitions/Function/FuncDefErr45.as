package Errors {

 interface _If1ArgRet
 {
    function fn(argA:Number);
 }

 class FuncDefErr45 implements _If1ArgRet
 {
    public function fn(argB:Number,argA:Number):String {};
 }
}
