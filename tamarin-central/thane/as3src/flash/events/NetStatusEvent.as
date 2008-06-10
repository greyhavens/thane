package flash.events {

public class NetStatusEvent extends Event
{
    public function NetStatusEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
    }
}

}
