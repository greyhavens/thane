package Errors {

 interface _If
 {
    function get fn():Number;
 }

 class FuncDefErr0 implements _If
 {
    var s:String;
   public function get fn() { return s; }
 }

}
