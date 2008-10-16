/**
 *	
 *  	Coach Mode for former LocalConnection
 *      Warning Message:Location change. Moved to flash.net package. 
 */
 
package Warnings {
   import flash.net.*;
   import flash.util.*;
 
   var LC:LocalConnection = new LocalConnection();

   LC.allowDomain=function(sendingDomain:String):*{
       return(sendingDomain=="www.domain.com");
   }

   LC.allowInsecureDomain = function(sendingDomain:String):*{
    return (sendingDomain == "www.domain.com");
   }
   LC.domain();
   
   LC.onStatus = function(infoObject:Object):void{
       trace("Local connection on successfully");
   }
     
 
		
}
