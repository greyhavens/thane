package Errors{

public class publicClassSuperref{
        var x:Number;

        public function publicClassSuperref(){
                                           
                                                  x=20;
                                                  }
        public function SuperNumber():Number{var d:Number = x;
                                     
                                      return d;
                                                  }


                              }
public class publicClassSubref extends publicClassSuperref{
        var y:Number;
         
        public function publicClassSubref(){
                                        
                                        y=super.SuperNumber();
                                        super();
                                        //return;
                                        }

        public function myNumber():Number{
                                   var z:Number=super.SuperNumber();
                                   
                                   return z;
                                  }
                                                     }
public class publicClassWithSuperSubrefErr{
       
        var pbSub:publicClassSubref = new publicClassSubref();
        
        public function getMyNumber(){
                            var B:Number = pbSub.myNumber();
                            
                            return B;
                                    }       
                                     }


}
                                  