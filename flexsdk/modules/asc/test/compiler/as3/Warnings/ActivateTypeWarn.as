/**
 *	
 *  	Warning Message: Use the 'EventType.ACTIVATE' const instead.
 */

package Warnings {

	import flash.events.EventType; 
	import flash.util.trace;
        import flash.events.*;

	trace(Event.ACTIVATE_TYPE);
		// Should generate a compiler error and a coach mode warning 
		// that this has been changed to EventType.ACTIVATE
}

