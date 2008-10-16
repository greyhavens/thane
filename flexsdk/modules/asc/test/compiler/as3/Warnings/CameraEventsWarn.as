/*********************************************************************************************Coach Mode Warning for Camera Class:Name of onActivity() event handler changed to activity
**and for onStatus EventHandler -- Replaced in the new event model by a StatusEvent object of type "status"(STATUS).******************************************************************* 
*******************************************************************************************/



package Warnings{
    import flash.media.Camera;
    import flash.util.trace;

    var myvar:Camera = Camera.getCamera();
    
    myvar.onActivity = function():void{
        trace("On Activity");}
    myvar.onStatus   = function():void{
        trace("On Status")}
}