package flash.events {

public class SecurityErrorEvent extends ErrorEvent
{
    public static const SECURITY_ERROR :String = "securityError";

    public function SecurityErrorEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false,
        text :String = "", id :int = 0)
    {
        super(type, bubbles, cancelable);
    }
}

}
