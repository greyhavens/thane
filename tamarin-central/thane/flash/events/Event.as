package flash.events {

public class Event
{
    public static const CONNECT :String = "connect";
    public static const CLOSE :String = "close";
    public static const COMPLETE:String = "complete";
    public static const UNLOAD:String = "unload";

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

}
