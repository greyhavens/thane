/**
 *	
 *  	Coach Mode for flash.display.SimpleButton
 */

package Warnings {

import flash.display.SimpleButton;
import flash.util.trace;
import flash.events.EventDispatcher;

var btn:SimpleButton = new SimpleButton()

btn._alpha;
btn._focusrect;
btn._height;
trace(btn._highquality);
btn._name;
btn._parent;
trace(btn._quality);
btn._rotation;
_soundbuftime=10;
trace(btn._soundbuftime);
trace(btn._target)
btn._url;
btn._visible;
btn._width;
btn._x;
btn._xmouse;
btn._xscale;
btn._y;
btn._ymouse;
btn._yscale;

trace(btn.getDepth())
btn.onDragOut = function():void{
    trace("OnDragOut");
}
btn.onDragOver = function():void{
    trace("OnDragOver");
}
btn.onKeyDown = function():void{
    trace("onKeyDown");
}
btn.onSetFocus = function():void{
    trace("onSetFocus");
}
btn.onKillFocus = function():void{
    trace("onKillfocus");
}
btn.onPress = function():void{
    trace("On press");
}
btn.onRelease = function():void{
    trace("On Release");
}
btn.onReleaseOutside = function():void{
    trace("On Release Outside");
}
btn.onRollOut = function():void{
    trace("On Roll Out");
}

}