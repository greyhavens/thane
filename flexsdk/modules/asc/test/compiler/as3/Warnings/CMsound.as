/**
 *	
 *  	Coach Mode for flash.media.Sound
 */

package Warnings {
	
	import flash.media.Sound;
	import flash.util.trace;
        import flash.events.EventDispatcher;
        import flash.events.EventType;
        import flash.events.EventType.COMPLETE;
        import flash.events.Event;
        import flash.display.Sprite;
	
	var sd:Sound = new Sound()
        var someEventDispatcher:EventDispatcher = new EventDispatcher();
	trace(sd.position)
        sd.duration
        sd.attachSound()
        sd.getBytesLoaded()
        sd.getBytesTotal()
        sd.getPan()
        sd.getTransform()
        sd.getVolume()
        sd.loadSound()
        sd.onID3 = function():void{
            
        };
        
         
        sd.onLoad = function(success:Boolean):void{
            if (success){
                trace("success");
            }
        };

        sd.onSoundComplete = function():void{
            trace("SoundId completed");
        };
        
	sd.setPan()
	sd.setTransform()
	sd.setVolume()
	sd.start()
        
	
}
