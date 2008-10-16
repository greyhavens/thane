/**
 *
 *  	Coach Mode for flash.display.Stage
 */

package Warnings {

	import flash.display.Stage
        import flash.display.StageScaleMode
        import flash.events.Event;
        import flash.events.EventDispatcher;
        import flash.util.trace;

	var stg:Stage = new Stage()
	var mylistener:*= new Object();
        var someEventDispatcher:EventDispatcher = new EventDispatcher();

        stg.scaleMode = "no scale";
	stg.showMenu
	stg.addListener()
	stg.removeListener()
        stg.onResize = function():void{}
        mylistener.onResize = function():void{
            trace("Stage size is now " + stg.width + " by " + stg.height);
        }

        function resizeListener(event:Event):void{
            trace(event.type);
        }
        someEventDispatcher.addEventListener(mylistener, resizeListener);

}