package Errors{

public class publicClassSuperreturn{
        var x:Number;

        public function publicClassSuperreturn(){
                                           
                                                  x=20;
                                                  }
        public function SuperNumber():Number{var d:Number = x;
                                     
                                      return d;
                                                  }


                              }
public class publicClassSubreturn extends publicClassSuperreturn{
        var y:Number;
         
        public function publicClassSubreturn(){
                                        return;
                                        super();
                                        //y=super.SuperNumber();
                                        
                                        //return;
                                        }

        public function myNumber():Number{
                                   var z:Number=super.SuperNumber();
                                   
                                   return z;
                                  }
                                                     }
public class publicClassWithSuperSubreturnErr{
       
        var pbSub:publicClassSubreturn = new publicClassSubreturn();
        
        public function getMyNumber(){
                            var B:Number = pbSub.myNumber();
                            
                            return B;
                                    }       
                                     }


}
                                  