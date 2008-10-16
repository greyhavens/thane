interface I{

                        function deffunc():String;                                   }

interface F{

                        function deffunc():String;                                   }


class A implements I, F{

                         public I function deffunc():String{             
                                    return"PASSED";

                        }
                         public F function deffunc():String{             
                                    return"PASSED";

                        }

}

