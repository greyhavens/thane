package flash.events {

public class TimerEvent extends Event
{
    public static const TIMER :String = "timer";
    public static const TIMER_COMPLETE :String = "timerComplete";

    public function TimerEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
    }
}

}
