//[Compiler] Warning #1092: Negative value will become a large positive value when assigned to a uint data type.

class C
{
   function f(v:uint)
   {
      var x:uint = -1;
      x = -1;
      v = -1;
   }
}

(new C).f(-1);
