/**
 *	
 *  	Warning Message: Use the 'Event.OPEN' const instead.
 */
package Warnings {

	import flash.events.EventType;
	import flash.util.trace;
        import flash.events.Event;

	trace(Event.OPEN_TYPE);  // should be EventType.Open	
}