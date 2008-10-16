function test(){
   thisError="no error";
   try{
       throw null;
   }
   catch(e:Error){
           thisError=e.toString();
           print (thisError+"FAILED!: Should have exited with uncaught exception.");
           }
   finally{}
    
  }
   
test();    
