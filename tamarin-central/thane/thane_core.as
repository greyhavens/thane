//
// $Id: $

package flash.errors {

public class MemoryError extends Error
{
}

public class IllegalOperationError extends Error
{
    public function IllegalOperationError (str :String = null)
    {
    }
}
}

package flash.events {

import flash.utils.Dictionary;

public class Event
{
    public static const CONNECT :String = "connect";
    public static const CLOSE :String = "close";

    public function Event (type :String, bubbles :Boolean = false, cancelable :Boolean = false)
    {
        if (bubbles || cancelable) {
            throw new Error("Not implemented");
        }
        _type = type;
    }

    public function get type () :String
    {
        return _type;
    }

    public function get target () :Object
    {
        return null;
    }

    public function clone () :Event
    {
        return new Event(_type);
    }

    private var _type :String;
}

public class EventDispatcher
{
    public function addEventListener (
        type :String, listener :Function, useCapture :Boolean = false,
        priority :int = 0, useWeakReference :Boolean = false) :void
    {
        if (useCapture || priority != 0 || useWeakReference) {
            throw new Error("Fancy addEventListener not implemented");            
        }

        var listeners :Array = _listenerMap[type] as Array;
        if (listeners == null) {
            _listenerMap[type] = listeners = new Array();

        } else if (-1 == _listenerMap.indexOf(listener)) {
            return;
        }
        listeners.push(listener);
    }

    public function removeEventListener (
        type :String, listener :Function, useCapture :Boolean = false) :void
    {
        if (useCapture) {
            throw new Error("Fancy removeListener not implemented");
        }
        var listeners :Array = _listenerMap[type] as Array;
        if (listeners != null) {
            var ix :int = listeners.indexOf(listener);
            if (ix >= 0) {
                listeners.splice(ix, 1);
            }
        }
    }

    public function dispatchEvent (event :Event) :Boolean
    {
        var listeners :Array = _listenerMap[event.type] as Array;
        for each (var listener :Function in listeners) {
            try {
                listener(event);
            } catch (err :Error) {
                trace("Event[" + event + "] dispatch error: " + err);
            }
        }
        // TODO: "A value of true unless preventDefault() is called on the event,
        //        in which case it returns false. "
        return true;
    }

    private var _listenerMap :Dictionary = new Dictionary();
}

public class TimerEvent extends Event
{
    public static const TIMER :String = "timer";
    public static const TIMER_COMPLETE :String = "timerComplete";

    public function TimerEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
    }
}
}


package flash.utils {

import flash.events.EventDispatcher;
import flash.events.TimerEvent;

import avmplus.*;

public function describeType (c :Object) :XML
{
    throw new Error("describeType() Not implemented");
}

public function getDefinitionByName (c :String) :Class
{
    return Domain.currentDomain.getClass(name);
}

public function getQualifiedClassName (c :*) :String
{
    return Domain.currentDomain.getClassName(c);
}

public function getQualifiedSuperclassName (c :*) :String
{
    throw new Error("getQualifiedSuperclassName() not implemented");
}

var timers :Dictionary = new Dictionary();
var timerIx :uint = 1;

public function setTimeout (closure :Function, delay :Number, ... arguments) :uint
{
    return createTimer(closure, delay, true, arguments);
}

public function setInterval (closure :Function, delay :Number, ... arguments) :uint
{
    return createTimer(closure, delay, false, arguments);
}

public function clearTimeout (id :uint): void
{
    destroyTimer(id);
}

public function clearInterval (id :uint): void
{
    destroyTimer(id);
}

public function getTimer () :int
{
    return System.getTimer();
}

function createTimer (closure :Function, delay :Number, timeout :Boolean, ... arguments) :uint
{
    var timer :Timer = new Timer(delay, timeout ? 1 : 0);
    var fun :Function = function (event :TimerEvent) :void {
        if (timeout) {
            destroyTimer(timerIx);
        }
        closure.apply(null, arguments);
    };
    timer.addEventListener(TimerEvent.TIMER, fun);
    timers[timerIx] = [ timer, fun ];
    return timerIx ++;
}

function destroyTimer (id :uint) :void
{
    var bits :Array = timers[id];
    if (bits) {
        bits[0].removeEventListener(TimerEvent.TIMER, bits[1]);
        bits[0].stop();
        delete timers[id];
    }
}

public interface IDataInput
{
    function readBytes(bytes :ByteArray, offset :uint=0, length :uint=0) :void;
    function readBoolean() :Boolean;
    function readByte() :int;
    function readUnsignedByte() :uint;
    function readShort() :int;
    function readUnsignedShort() :uint;
    function readInt() :int;
    function readUnsignedInt() :uint;
    function readFloat() :Number;
    function readDouble() :Number;
    function readUTF() :String;
    function readUTFBytes(length :uint) :String;
    function get bytesAvailable() :uint;
    function get endian() :String;
    function set endian(type :String) :void;
}

public interface IDataOutput
{
    function writeBytes(bytes :ByteArray, offset :uint = 0, length :uint = 0) :void;
    function writeBoolean(value :Boolean) :void;
    function writeByte(value :int) :void;
    function writeShort(value :int) :void;
    function writeInt(value :int) :void;
    function writeUnsignedInt(value :uint) :void;
    function writeFloat(value :Number) :void;
    function writeDouble(value :Number) :void;
    function writeUTF(value :String) :void;
    function writeUTFBytes(value :String) :void;
    function get endian() :String;
    function set endian(type :String) :void;
}
}
