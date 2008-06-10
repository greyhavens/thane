//
// $Id: $

package flash.display {

import flash.events.Event;
import flash.events.EventDispatcher;

import flash.errors.IllegalOperationError;

public class LoaderInfo extends EventDispatcher
{
    static function maybeCreateLoaderInfo () :LoaderInfo
    {
        if (Thanette.getBridgeHome() != null) {
            return new LoaderInfo();
        }
        return null;
    }

    public function get sharedEvents () :EventDispatcher
    {
        return Thanette.getBridgeHome();
    }

    override public function dispatchEvent (event :Event) :Boolean
    {
        throw new IllegalOperationError("calls to dispatchEvent() on LoaderInfo are not allowed");
    }

}

}
