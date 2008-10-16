package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr25 implements _If1Arg
 {
    public function fn(argA:String):Number {};
 }
}
