package Errors {

 interface _If1Arg
 {
    function fn(argA:Number);
 }

 class FuncDefErr0 implements _If1Arg
 {
    public function fn(argA:String) {};
 }

}

