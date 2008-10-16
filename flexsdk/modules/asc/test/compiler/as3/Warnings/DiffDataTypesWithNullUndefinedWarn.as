package warnings {

   public class aClass { }

 

   public class test {

       public function tryIt(i:int, b:Boolean, u:uint, n:Number, a:aClass):void

       {

                        if (i == undefined)

                              trace("impossible");

                        if (i === undefined)

                              trace("impossible");

                        if (i != undefined)

                              trace("impossible");

                        if (i >= undefined)

                              trace("impossible");

                        if (i <= undefined)

                              trace("impossible");

                        if (i > undefined)

                              trace("impossible");

                        if (i < undefined)

                              trace("impossible");

                  
                        if (i == null)

                              trace("impossible");

                        if (i === null)

                              trace("impossible");

                        if (i != null)

                              trace("impossible");

                        if (i >= null)

                              trace("impossible");

                        if (i <= null)

                              trace("impossible");

                        if (i > null)

                              trace("impossible");

                        if (i < null)

                              trace("impossible");

                        if (b == undefined)

                              trace("impossible");

                        if (b === undefined)

                              trace("impossible");

                        if (b != undefined)

                              trace("impossible");

                        if (b >= undefined)

                              trace("impossible");

                        if (b <= undefined)

                              trace("impossible");

                        if (b > undefined)

                              trace("impossible");

                        if (b < undefined)

                              trace("impossible");

                        if (b == null)

                              trace("impossible");

                        if (b === null)

                              trace("impossible");

                        if (b != null)

                              trace("impossible");

                        if (b >= null)

                              trace("impossible");

                        if (b <= null)

                              trace("impossible");

                        if (b > null)

                              trace("impossible");

                        if (b < null)

                              trace("impossible");


                         

                        if (u == undefined)

                              trace("impossible");

                        if (u === undefined)

                              trace("impossible");

                        if (u != undefined)

                              trace("impossible");

                        if (u >= undefined)

                              trace("impossible");

                        if (u <= undefined)

                              trace("impossible");

                        if (u > undefined)

                              trace("impossible");

                        if (u < undefined)

                              trace("impossible");


                        if (u == null)

                              trace("impossible");

                        if (u === null)

                              trace("impossible");

                        if (u != null)

                              trace("impossible");

                        if (u >= null)

                              trace("impossible");

                        if (u <= null)

                              trace("impossible");

                        if (u > null)

                              trace("impossible");

                        if (u < null)

                              trace("impossible");

                        if (n == undefined)

                              trace("impossible");

                        if (n === undefined)

                              trace("impossible");

                        if (n != undefined)

                              trace("impossible");

                        if (n >= undefined)

                              trace("impossible");

                        if (n <= undefined)

                              trace("impossible");

                        if (n > undefined)

                              trace("impossible");

                        if (n < undefined)

                              trace("impossible");
                        
                         if (n == null)

                              trace("impossible");

                        if (n === undefined)

                              trace("impossible");

                        if (n != null)

                              trace("impossible");

                        if (n >= null)

                              trace("impossible");

                        if (n <= null)

                              trace("impossible");

                        if (n > null)

                              trace("impossible");

                        if (n < null)

                              trace("impossible");

                        
                         if (a == undefined)

                              trace("impossible");

                        if (a === undefined)

                              trace("impossible");

                        if (a != undefined)

                              trace("impossible");

                        if (a >= undefined)

                              trace("impossible");

                        if (a <= undefined)

                              trace("impossible");

                        if (a > undefined)

                              trace("impossible");

                        if (a < undefined)

                              trace("impossible");






// repeat all cases above with each argument

                  // repeat all the above cases with null instead of undefined.

                  

                  // all cases should log a warning with the exception of the comparisions of a with null.  All of those

                  //  comparisions should be legal.

            }

      }

      var k:test = new test();
       
      k.tryIt(10,true,10,10.00,aClass);
 

}
