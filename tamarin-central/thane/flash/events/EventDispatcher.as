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

    private var _listenerMap :Dictionary = new Dictionary();
}

}
