package flash.events {

import flash.utils.Dictionary;

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

}
