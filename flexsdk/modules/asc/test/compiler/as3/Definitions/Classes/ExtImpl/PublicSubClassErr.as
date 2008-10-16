package Errors{
        public interface PublicInt{
                                  function MyString():String;
                                  }
        public interface PublicInt2{
                                  function MyString():String;
                                  function MyNegInteger():int;
                                   }
        public interface PublicInt3 extends PublicInt,PublicInt2{
                                  function MyUnsignedInteger():uint;
                                  function MyNegativeInteger():int;
                                                                 }
        public class PublicSuperClass{
                                   public function MySuperBoolean():Boolean{return true;}
                                   internal function MySuperNumber():Number{return 10;}
                            public static function MySuperStaticDate():Date {return new Date(0);}


                                     }

        public class PublicSubClass extends PublicSuperClass implements PublicInt3{
                              var s = new PublicSuperClass();
                                 public function MyString():String{
                                                                    return "Hi!";
                                                                    }

                                 public function MyNegInteger():int{
                                                                    var a:int = -100;
                                                                    return a;
                                                                       }
                                  public function MyUnsignedInteger():uint{
                                                                    var postint:uint =100;
                                                                    return postint;
                                                                  }
                                  public function MyNegativeInteger():int{
                                                                    var negint:int = -100000;
                                                                    return negint;
                                                                           }
                                  public function RetMySuperNumber():Number{return MySuperNumber();}

                                                            }


                                                 }
import Errors.*;
var obj = new PublicSubClass();
var PubInt:PublicInt=obj;

var PubInt2:PublicInt2=obj;

var PubInt3:PublicInt3=obj;
//print(PubInt3.MyString());//This should throw an ambiguous reference error --Subha Subramanian
PubInt3.MyString();//This should throw an ambiguous reference error --Subha Subramanian