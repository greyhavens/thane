package testInternalClassConsWithReturnErr{

       internal class intClassWithParamConsWithReturnErr{
                private var x:Number;
                private var y:Number;
                public var z:Number;
                public function intClassWithParamConsWithReturnErr(a:Number,b:Number){
                                                                         x = a;
                                                                         y = b;
                                                                         return z=x+y; 
                                                                        }

                                        

               public function Add():Number{
                                    
                                 var H:Number=x+y;
                                    
                                    return H;
                                     }
                                          }
                              
 
public class wrapintClassWithReturn{



var inWthCReturn:intClassWithParamConsWithReturnErr=new intClassWithParamConsWithReturnErr(20,40); 
                                
                                 public function getAdd(){
                                                          var R = inWthCReturn.Add();
                                                          
                                                          return R;
                                                          }
                                              }
}