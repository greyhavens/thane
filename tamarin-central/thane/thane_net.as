//
// $Id: $

package flash.errors {

public class IOError extends Error
{
    public function IOError(message :String = "")
    {
        super(message);
    }
}
}

package flash.events {

import flash.utils.Dictionary;

public class NetStatusEvent extends Event
{
    public function NetStatusEvent (
        type :String, bubbles :Boolean = false, cancelable :Boolean = false, text :String = "")
    {
        super(type, bubbles, cancelable);
    }
}

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

package flash.net
{

public class ObjectEncoding
{
    public static const AMF0 :uint = 0;
    public static const AMF3 :uint = 3;
    public static const DEFAULT :uint = AMF3;
}
}
