package p {

  class test {
   
       function foo2(a:Number =1, ...rest) { print(a) }
       function test() 
       { 	
  	foo2(null,null);
       }
 }
  
 var obj = new test();
 
}


