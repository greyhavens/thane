/*
 * native keyword cannot be used with a function
 */

 package Errors {
 
  interface inter {
     native final function foo();
  }
 
  class FuncDefrt98 implements inter {
 
   public native final function foo() {}
 
  }
 }

