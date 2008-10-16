/*

  Error code :1176 Comparison between a value with static type '_' and a possibly unrelated type '_'

*/

package {

   class foo { }

   function testit() {

            var x:foo = new foo();

            if (x == 2)

                  return "Cannot happen!"

      }

      

      testit();

}

 

 