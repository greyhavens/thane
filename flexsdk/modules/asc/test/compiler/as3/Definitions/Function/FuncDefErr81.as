package Errors {
 
 interface _If1ArgRet
 {
    function fn(argA:Number);
 }

 class FuncDefErr81 implements _If1ArgRet
 {
   public function fn(argA:Number):String {};
 }
}
