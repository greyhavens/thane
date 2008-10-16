/**
 *	
 *  	Coach Mode for flash.net.SharedObject
 */

package Warnings {
	
        import flash.events.EventDispatcher;
	import  flash.net.SharedObject;
        
        import flash.util.trace;
        
	
	var so:SharedObject = new SharedObject()
        var someEventDispatcher:EventDispatcher= new EventDispatcher();

        so.onStatus=function(){
            trace("status of the object");
        }
        
        so.getSize()
	
	
	
}