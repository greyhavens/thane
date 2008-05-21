package flash.events {

public class TextEvent extends Event
{
    public var text :String;

    public function TextEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
        this.text = text;
    }
}

}
