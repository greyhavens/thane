/**
 *	
 *  	Coach Mode for flash.net.XMLSocket
 */

package Warnings {

import flash.net.XMLSocket
import flash.events.EventDispatcher;
import flash.events.Event;
import flash.events.EventType.*;
import flash.util.trace;

var hostName:String = "localhost";
var port:uint=8080; 
var xmlSocket:XMLSocket= new XMLSocket()
xmlSocket.connect(hostName,port);
var someEventDispatcher:EventDispatcher = new EventDispatcher();


xmlSocket.onXML = function():void{}
xmlSocket.onClose = function():void{}

function closeListener(event:Event):void{
    trace(event.type); // output: close
}

someEventDispatcher.addEventListener(XMLSocket.onClose,closeListener);

xmlSocket.onConnect = function():void{}

function connectListener(event:Event):void{
    trace(event.type); // output: connect
}

someEventDispatcher.addEventListener(EventType.onConnect, connectListener);
someEventDispatcher.addEventListener(xmlSocket.onConnect,connectListener);
someEventDispatcher.addEventListener(XMLSocket.onConnect,connectListener);

xmlSocket.onData = function():void{}

}