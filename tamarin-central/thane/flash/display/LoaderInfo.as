//
// $Id: $

package flash.display {

import flash.events.Event;
import flash.events.EventDispatcher;

import flash.errors.IllegalOperationError;

public class LoaderInfo extends EventDispatcher
{
    public static var __currentBridge :EventDispatcher = null;

    static function maybeCreateLoaderInfo () :LoaderInfo
    {
        if (__currentBridge == null) {
            return null;
        }
        var info :LoaderInfo = new LoaderInfo();
        info._bridge = __currentBridge;
        return info;
    }

    public function get sharedEvents () :EventDispatcher
    {
        return _bridge;
    }

    override public function dispatchEvent (event :Event) :Boolean
    {
        throw new IllegalOperationError("calls to dispatchEvent() on LoaderInfo are not allowed");
    }

    protected var _bridge :EventDispatcher;
}

}
