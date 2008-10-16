package Errors {

 interface _If1ArgRet
 {
    function fn(argA:Number);
 }

 class FuncDefErr54 implements _If1ArgRet
 {
    public function fn(argB:String,argA:String):String {};
 }
}
