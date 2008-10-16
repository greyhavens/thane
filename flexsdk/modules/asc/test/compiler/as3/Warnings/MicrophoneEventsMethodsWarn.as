/*******************************************************************************************
**************************Coach Warning for Microphone Class********************************
*******************************************************************************************/

package Warnings{
    import flash.media.Microphone;
    import flash.util.*;

    var my_microphone:Microphone = new Microphone();
    Microphone.get();

    var my_mc:Microphone = Microphone.getMicrophone();

    my_mc.onActivity = function(active:Boolean):void{
        trace("Microphone is active");
    }
    
    my_mc.onStatus = function():void{
        trace("On status");
    }

    my_microphone.setGain(100);
    my_microphone.setRate();
    
}