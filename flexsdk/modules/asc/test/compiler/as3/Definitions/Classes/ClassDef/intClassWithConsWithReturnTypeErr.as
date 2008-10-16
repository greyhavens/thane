package Errors{

       internal class intClassWithConsWithReturnTypeErr{
                private var x:Number;
                private var y:Number;

                public function intClassWithConsWithReturnTypeErr(a:Number,b:Number):Number{
                                                                         x = a;
                                                                         y = b;
                                                                        }

                                        

                public function Add():Number{
                                     var z = x+y;
                                     return z;
                                     }
                                          }
                              
 
       public class wrapintClassConsWithReturnType{
       

var intWithParCons:intClassWithConsWithReturnTypeErr=new intClassWithConsWithReturnTypeErr(20,40); 
                                  

                                 public function getAdd(){
                                                          var R = intWithParCons.Add();
                                                          return R;
                                                         }
                                              }
                                             }