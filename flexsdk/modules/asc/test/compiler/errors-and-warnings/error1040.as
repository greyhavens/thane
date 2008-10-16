// [Compiler] Error #1040: Duplicate label definition
class C {
   // function i CAN have the same name as the label because they are different namespaces
   // (break i; refers ONLY to the label namespace
   public function set i(n:Number) {
      // duplicate (ambiguous) labels here (because they are nested)
      i:{
         i:{
            print("world");
            for (var i=0; i<100; i++) {
               if (i == 5) {
                  break i;
               }
               print("hello");
            }
         print("after for");   
         }
      print("after block");
      }

      // yes, another i label, but the label's scope is only valid in
      // the immediately defined block, not the scope it's declared in
      // (you can only say 'break i;' WITHIN the defined block)
      i:{}

      print("L2");
      i=n;
   }
}
