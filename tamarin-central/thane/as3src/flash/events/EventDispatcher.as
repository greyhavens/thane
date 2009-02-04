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

import flash.utils.Dictionary;

public class EventDispatcher implements IEventDispatcher
{
    public function addEventListener (
        type :String, listener :Function, useCapture :Boolean = false,
        priority :int = 0, useWeakReference :Boolean = false) :void
    {
        if (useCapture || priority != 0) {
            throw new Error("Fancy addEventListener not implemented");            
        }

        if (useWeakReference) {
            // we can't do this properly with our current data structure, just ignore for now
        }

        var listeners :Array = _listenerMap[type] as Array;
        if (listeners == null) {
            _listenerMap[type] = listeners = new Array();

        } else if (-1 != listeners.indexOf(listener)) {
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

        if (listeners == null) {
            return true;
        }

        for each (var listener :Function in listeners.concat()) {
            try {
                listener(event);
            } catch (err :Error) {
                var errStr :String = err.getStackTrace();
                if (errStr == null) {
                    errStr = err.message;
                }
                trace(errStr);
            }
            if (event.isPropagationStopped(true)) {
                break;
            }
        }
        return !event.isDefaultPrevented();
    }

    public function hasEventListener (type :String) :Boolean
    {
        if (type in _listenerMap) {
            return _listenerMap[type].length > 0;
        }
        return false;
    }

    public function toString () :String
    {
        return "[EventDispatcher]";
    }

    private var _listenerMap :Dictionary = new Dictionary();
}

}
