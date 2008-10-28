package flash.events {

public class HTTPStatusEvent extends ErrorEvent
{
    public static const HTTP_STATUS :String = "httpStatus";

    public function HTTPStatusEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false,
        status :int = 0)
    {
        super(type, bubbles, cancelable);
        _status = status;
    }
    
    public function get status () :int
    {
        return _status;
    }

    protected var _status :int;
}

}
