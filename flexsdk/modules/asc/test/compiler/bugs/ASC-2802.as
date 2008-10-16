 class foo {
   var _cp:int=0
   function get currentPos():int {
     return _cp;
   }
   /*
   function set currentPos(cp:int):void {
     _cp=cp
   }
   */
   function movepos():int {
// return currentPos=currentPos+1
     return curentPos++
   }
 }
 var f:foo=new foo()
 trace(f.movepos()) 
