package testinternalClassWithinternalCons{

       internal class internalClassWithinternalCons{
                private var x:Number=0;
                private var y:Number=0;

                internal function internalClassWithinternalCons(a:Number,b:Number){
                                                                           x = a;
                                                                           y = b;
                                                                           }
                                              

                                        

                public function Add():Number{
                                     var z = x+y;
                                     return z;
                                            }
                                             

                                             }

       public class wrapinternalClassWithinternalCons{

    var intClintCons:internalClassWithinternalCons=new internalClassWithinternalCons(20,40);
                              
                public function getAdd():Number{
                                      return intClintCons.Add();
                                                }
                                                      }
       
                                                }