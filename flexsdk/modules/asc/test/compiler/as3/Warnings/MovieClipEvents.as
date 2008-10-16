package Warnings {
		import flash.util.trace;
		import flash.events.*;
		import flash.display.MovieClip;
		import flash.display.DisplayObject;
		import flash.geom.Point;
		var myPoint:Point = new Point(10, 10);
		
		var base:MovieClip = new MovieClip();
		var a_mc:MovieClip = new MovieClip();

		a_mc.onRelease = function():void { trace("onRelease ..."); };
	    a_mc.onMouseDown = function():void  { trace("onMouseDown"); };
		a_mc.onData = function():void  { trace("onData"); };
		a_mc.onDragOut = function():void  { trace("onDragOut"); };
		a_mc.onDragOver = function():void  { trace("onDragOver"); };
		a_mc.onEnterFrame = function():void  { trace("onEnterFrame"); };
		a_mc.onKeyDown = function():void  { trace("onKeyDown"); };
		a_mc.onKeyUp = function():void  { trace("onKeyUp"); };
		a_mc.onKillFocus = function():void  { trace("onKillFocus"); };
		a_mc.onLoad = function():void  { trace("onLoad"); };
		a_mc.onMouseMove = function():void  { trace("onMouseMove"); };
		a_mc.onMouseUp = function():void  { trace("onMouseUp"); };
		a_mc.onPress = function():void  { trace("onPress"); };
		a_mc.onReleaseOutside = function():void  { trace("onReleaseOutside"); };
		a_mc.onRollOut = function():void  { trace("onRollOut"); };
		a_mc.onRollOver = function():void  { trace("onRollOver"); };
		a_mc.onSetFocus = function():void  { trace("onSetFocus"); };
		a_mc.onUnload = function():void  { trace("onUnload"); };
		base.addChild(a_mc);
}