// [Compiler] Error #1012: 'static' attribute may only be used on definitions inside a class

package {
   class A {

     function f() {
       // this should not be allowed
       static var sv = 1;

       // this should not be allowed
       static class B {}

       // this should not be allowed
       function g() {
         static class B {}
       }
     }

     static class B {} // this should be allowed... for now
  }
}
