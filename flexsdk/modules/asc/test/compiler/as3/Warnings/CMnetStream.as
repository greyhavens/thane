/**
 *	
 *  	Coach Mode: NetStream
 */

package Warnings {
         
	 import flash.net.NetStream
         import flash.util.trace
		
	 var a:NetStream = new NetStream();
         
         
	 
	 a.currentFps;
	 
         a.onStatus=function(){
             trace("on status");
         }
	 
	 a.setBufferTime();
		
		
}

