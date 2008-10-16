/**
 *	
 *  	Coach Mode for Object()
 *		Properties and functions tested:
 *		registerClass()
 *		addProperty()
 *		watch()  
 *		unwatch()
 *		__proto__ 
 *		__resolve
 */

import flash.display.MovieClip;
import flash.text.TextField;
import flash.util.trace;

var obj:Object = new Object();
Object.registerClass();   //  Testing AS 2.0 registerClass method

class Speedy extends Object{

	public var speed:Number;
	public function Speedy()
	{
		super();
	}

	public function setSpeed(inSpeed:Number):void
	{
		speed = inSpeed;
	}

	public function getSpeed():Number
	{
		return speed;
	}
}

var Speedster:Speedy = new Speedy();
Speedster.addProperty("speed", this.setSpeed, this.getSpeed);  //  Testing AS 2.0 addProperty
Speedster.speed = 100;

var speedWatcher:Function = function(prop:String, oldVal:Number, newVal:Number, speedLimit:Number):Number {
	if (newVal > speedLimit) {
		trace ("You are speeding.");
	}
	else {
		trace ("You are not speeding.");
	}		
	return newVal;
}
Speedster.watch("speed", speedWatcher, 55);  //  Testing AS 2.0 watch method
Speedster.speed = 54; 
Speedster.speed = 57; 
Speedster.unwatch("speed");
Speedster.speed = 54; 

obj.__resolve = function():String  //  Testing AS 2.0 __resolve method
{
	return "Hello World!";
};

var oneField:TextField = new TextField();

obj.prototype = oneField.__proto__;		//  Testing AS 2.0 __proto__ 
trace (obj.property1); 