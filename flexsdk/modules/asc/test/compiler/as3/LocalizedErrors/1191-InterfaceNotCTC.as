/*
  Error Code:1191 Interface expression is not a compile-time constant: _ 

*/





package {

   class a {}

   

 

   function nonCompileTimeConstantFunc():Class { 

if (Math.random() > 0.5)

      return a;

else

      return b;

   }

   interface goo extends nonCompileTimeConstantFunc() { }


 

}
