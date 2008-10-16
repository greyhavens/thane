package Errors{

public class publicClassSuperthis{
        var x:Number;

        public function publicClassSuperthis(){
                                           
                                                  x=20;
                                                  }
        public function SuperNumber():Number{var d:Number = x;
                                     
                                      return d;
                                                  }


                              }
public class publicClassSubthis extends publicClassSuperthis{
        var y:Number;
         
        public function publicClassSubthis(){
                                        this.y=super.SuperNumber();
                                        super();
                                        //y=super.SuperNumber();
                                        
                                        //return;
                                        }

        public function myNumber():Number{
                                   var z:Number=super.SuperNumber();
                                   
                                   return z;
                                  }
                                                     }
public class publicClassWithSuperSubthisErr{
       
        var pbSub:publicClassSubthis = new publicClassSubthis();
        
        public function getMyNumber(){
                            var B:Number = pbSub.myNumber();
                            
                            return B;
                                    }       
                                     }


}
                                  