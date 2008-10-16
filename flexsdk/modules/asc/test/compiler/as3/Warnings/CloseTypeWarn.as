/**
 *
 *  	Warning Message: Use the 'EventType.CLOSE' const instead.
 */

package Warnings {

	import flash.events.Event;
        import flash.util.trace;
	import flash.events.EventDispatcher;

        var someEventDispatcher:EventDispatcher = new EventDispatcher();
        someEventDispatcher.addEventListener(Event.CLOSE_TYPE,closeListener);

        public function closeListener(event:Event){
           trace(event.type);
        }
}

