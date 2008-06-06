package {

import flash.events.Event;

public class TraceEvent extends Event
{
    public static const TRACE :String = "trace";

    public function TraceEvent (type :String, bubbles :Boolean = false,
                                cancelable :Boolean = false, trace :Array = null)
    {
        super(type, bubbles, cancelable);

        _trace = trace;
    }

    public function get trace () :Array
    {
        return _trace;
    }

    protected var _trace :Array = null;
}
}
