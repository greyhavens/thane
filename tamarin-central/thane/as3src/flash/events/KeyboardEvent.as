package flash.events {

/** This class mimics the flash class in order to compile whirled. Unused methods and 
 *  members are not present. */
public class KeyboardEvent extends Event
{
    public static const KEY_UP :String = "keyUp";
    public static const KEY_DOWN :String = "keyDown";

    public function KeyboardEvent (type :String)
    {
        super(type);
    }
}

}
