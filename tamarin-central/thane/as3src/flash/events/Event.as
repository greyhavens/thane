package flash.events {

import avmplus.StringBuilder;

public class Event
{
    public static const CONNECT :String = "connect";
    public static const CLOSE :String = "close";
    public static const COMPLETE:String = "complete";
    public static const UNLOAD:String = "unload";

    public function Event (type :String, bubbles :Boolean = false, cancelable :Boolean = false)
    {
        _type = type;
        if (bubbles) {
            _flags |= BUBBLES;
        }
        if (cancelable) {
            _flags |= CANCELABLE;
        }
    }

    public function get type () :String
    {
        return _type;
    }

    public function get target () :Object
    {
        return null;
    }

    public function get bubbles () :Boolean
    {
        return (_flags & BUBBLES) != 0;
    }

    public function get cancelable () :Boolean
    {
        return (_flags & CANCELABLE) != 0;
    }

    public function clone () :Event
    {
        return new Event(_type);
    }

    public function preventDefault () :void
    {
        if ((_flags & CANCELABLE) != 0) {
            _flags |= DEFAULT_PREVENTED;
        }
    }

    public function isDefaultPrevented () :Boolean
    {
        return (_flags & DEFAULT_PREVENTED) != 0
    }

    public function stopImmediatePropagation () :void
    {
        _flags |= STOP_IMMEDIATE_PROPAGATION;
        _flags |= STOP_PROPAGATION;
    }

    public function formatToString (className :String, ... arguments) :String
    {
        var s :StringBuilder = new StringBuilder();
        s.append("[");
        s.append(className);
        var self :Object = this;
        arguments.forEach (function (name :String, ... unused) {
            s.append(" ");
            s.append(name);
            s.append("=");
            s.append(self[name]);
        });
        s.append("]");
        return s.toString();
    }

    public function toString () :String
    {
        return formatToString("Event", "type", "bubbles", "cancelable");
    }

    internal function isPropagationStopped (immediate :Boolean) :Boolean
    {
        if (immediate) {
            return (_flags & STOP_IMMEDIATE_PROPAGATION) != 0;
        }
        return (_flags & STOP_PROPAGATION) != 0;
    }

    private var _type :String;
    private var _flags :int;

    private static const BUBBLES :int = 1 << 0;
    private static const CANCELABLE :int = 1 << 1;
    private static const DEFAULT_PREVENTED :int = 1 << 2;
    private static const STOP_IMMEDIATE_PROPAGATION :int = 1 << 3;
    private static const STOP_PROPAGATION :int = 1 << 4;
}

}
