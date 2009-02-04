//
// $Id$
//
// Thane - an enhancement of Mozilla/Adobe's Tamarin project with partial Flash Player compatibility
// Copyright (C) 2008-2009 Three Rings Design, Inc.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the following disclaimer in the documentation and/or other materials provided
//    with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
