package Errors {

 interface _If1ArgRet
 {
    function fn(argA:Number);
 }

 class FuncDefErr42 implements _If1ArgRet
 {
    public function fn(argA:String,argB:String):String {};
 }
}
