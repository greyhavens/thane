/*

  Error #1160: 'native' attribute may not be used an interface definitions

*/






package NativeUsedInIntDefErr{
    interface myinterface{
        native function myfunc():string;
     }
}