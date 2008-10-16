/**
 *	
 *  	Warning Message: Use the 'FocusEvent.KEY_FOCUS_CHANGE' const instead.
 */

package Warnings {

	import flash.events.FocusEvent;	
	import flash.display.MovieClip;
	import flash.util.trace;

	var dispMov:MovieClip = new MovieClip();
	function keyFocListener(event:FocusEvent):void {
		trace(event.type); // output: keyFocusChange
	} 	
	dispMov.addEventListener(FocusEvent.KEY_FOCUS_CHANGE_TYPE, keyFocListener); 			
}

