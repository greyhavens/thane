package A
{
public interface I
{
function get length():uint;
}

public interface L
{
function debug(msg:String, ...rest):void;
}

[asjfh(name="k;lkd;fa", type="mx.sfahawtaga")]
public class C implements I
{
    /**
    * Returns number of items in the data provider.
    *
    */
    [Event(name="modelChanged", type="mx.events.ModelChangedEvent")]
    public function get length():uint
    {
		return 0;
    }
    [metadataForS (SKeyName="SKeyValue")]
    private var s;
}

/**
  This is the logger that is used within the logging framework.
  It will dispatch events for each message logged using the log method
  @private
*/
public class Logger implements L
{
    /**
    * Returns number of items in the data provider.
    *
    */
    [Event(name="modelChanged", type="mx.events.ModelChangedEvent")]
    public function debug(msg:String, ...rest):void
    {
    }
}

}
