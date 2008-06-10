package flash.events {

public class IOErrorEvent extends ErrorEvent
{
    public static const IO_ERROR :String = "ioError";

    public function IOErrorEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false,
        text :String = "", id :int = 0) 
    {
        super(type, bubbles, cancelable);
    }
}

}
