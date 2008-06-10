package flash.events {

/** This class mimics the flash class in order to compile whirled. Unused methods and 
 *  members are not present. */
public class MouseEvent extends Event
{
    static public const CLICK :String = "click";

    public function MouseEvent (type :String)
    {
        super(type);
    }
}

}
