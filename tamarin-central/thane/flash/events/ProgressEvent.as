package flash.events {

public class ProgressEvent extends Event
{
    public static const SOCKET_DATA :String = "socketData";

    public function ProgressEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
    }
}

}
