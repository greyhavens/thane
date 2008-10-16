/*Two super statements in a call should throw compiler error*/

package Errors{

public class publicClasstwoSuper{
        var x:Number=0;
        
        public function publicClasstwoSuper(){
                                           
                                                  x=a;
                                                 
                                                  }
        public function SuperNumber():Number{var d:Number = x;
                                     
                                      return d;
                                                  }


                              }
public class publicClasstwoSub extends publicClasstwoSuper{
        
        var pbSup1:publicClasstwoSuper=new publicClasstwoSuper(); 
        public function publicClasstwoSub(){
                                        
                                        
                                        super();
                                        super();
                                        
                                        return;
                                        }

        public function myNumber():Number{
                                   var z:Number=super.SuperNumber();
                                   
                                   return z;
                                  }
                                                     }
public class publicClassWithtwoSuperSubErr{
        
        
        var pbSub:publicClasstwoSub = new publicClasstwoSub();
        
        public function getMyNumber(){
                            var B:Number = pbSub.myNumber();
                            
                            return B;
                                    } 
           
        
        
                      
                                     }


}
                                  