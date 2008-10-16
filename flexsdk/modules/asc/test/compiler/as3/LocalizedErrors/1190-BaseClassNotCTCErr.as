/*
  Error Code:1190 Base class expression is not a compile-time constant: _ 

*/
package {
   class a {}
   class b {}
   class c {
   static function nonCompileTimeConstantFunc():Class { 
     if (Math.random() > 0.5)
       return a;
     else
       return b;
   }
   }
   class goo extends c.nonCompileTimeConstantFunc() { }
}
