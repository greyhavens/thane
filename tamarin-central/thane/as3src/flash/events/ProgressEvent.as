package flash.events {

public class ProgressEvent extends Event
{
    public static const SOCKET_DATA :String = "socketData";
    public static const PROGRESS :String = "progress";

    public function ProgressEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false,
        bytesLoaded :Number = 0, bytesTotal :Number = 0)
    {
        _bytesLoaded = bytesLoaded;
        _bytesTotal = bytesTotal;

        super(type, bubbles, cancelable);
    }

    public function get bytesLoaded () :Number
    {
        return _bytesLoaded;
    }

    public function set bytesLoaded (value :Number) :void
    {
        _bytesLoaded = value;
    }

    public function get bytesTotal () :Number
    {
        return _bytesTotal;
    }

    public function set bytesTotal (value :Number) :void
    {
        _bytesTotal = value;
    }

    public override function clone () :Event
    {
        return new ProgressEvent(type, bubbles, cancelable, bytesLoaded, bytesTotal);
    }

    public override function toString () :String
    {
        return formatToString("ProgressEvent", "type", "bubbles", "cancelable",
                              "bytesLoaded", "bytesTotal");
    }

    protected var _bytesLoaded :Number;
    protected var _bytesTotal :Number;
}
}
