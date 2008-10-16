package Errors{

public class publicClassSuperthrow{
        var x:Number;

        public function publicClassSuperthrow(){

                                                  x=20;
                                                  }
        public function SuperNumber():Number{var d:Number = x;

                                      return d;
                                                  }


                              }
public class publicClassSubthrow extends publicClassSuperthrow{
        var y:Number;

        public function publicClassSubthrow(){
//                                        throw(print("Error!"));
                                        throw("Error!");
                                        super();
                                        //y=super.SuperNumber();

                                        //return;
                                        }

        public function myNumber():Number{
                                   var z:Number=super.SuperNumber();

                                   return z;
                                  }
                                                     }
public class publicClassWithSuperSubthrowErr{

        var pbSub:publicClassSubthrow = new publicClassSubthrow();

        public function getMyNumber(){
                            var B:Number = pbSub.myNumber();

                            return B;
                                    }
                                     }


}
                                  