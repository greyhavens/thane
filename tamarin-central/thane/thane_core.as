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

    // TODO: This event must not exist
    public static const ENTER_FRAME :String = "enterFrame";

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
            throw new Error("Not implemented");            
        }

        var listeners :Array = _listenerMap[type] as Array;
        if (listeners == null) {
            _listenerMap[type] = listeners = new Array();

        } else if (-1 == _listenerMap.indexOf(listener)) {
            return;
        }
        _listenerMap.push(listener);
    }

    public function removeEventListener (
        type :String, listener :Function, useCapture :Boolean = false) :void
    {
        if (useCapture) {
            throw new Error("Not implemented");
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
        return true;
    }

    private var _listenerMap :Dictionary = new Dictionary();
}

public class TimerEvent extends Event
{
    public static const TIMER :String = "timer";

    public function TimerEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
    }
}
}


package flash.utils {

import flash.events.EventDispatcher;

public function describeType (c :Object) :XML
{
    return null;
}

public function getDefinitionByName (c :String) :Class
{
    return null;
}

public function getQualifiedClassName (c :Object) :String
{
    return null;
}

public function getTimer () :uint
{
    return 1;
}

public class Timer extends EventDispatcher
{
    public function Timer (n :uint)
    {
    }

    public function get running () :Boolean
    {
        return _running;
    }

    public function start () :void
    {
        _running = true;
    }

    public function stop () :void
    {
        _running = false;
    }

    public function reset () :void
    {
        stop();
    }

    private var _running :Boolean;
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
