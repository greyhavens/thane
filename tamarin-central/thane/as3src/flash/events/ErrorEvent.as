package flash.events {

public class ErrorEvent extends TextEvent
{
    public static const ERROR :String = "error";

    public function ErrorEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false,
        text :String = "", id :int = 0)
    {
        super(type, bubbles, cancelable, text);

        _errorId = id;
    }

    public function get errorID () :int
    {
        return _errorId;
    }

    protected var _errorId :int;
}

}
