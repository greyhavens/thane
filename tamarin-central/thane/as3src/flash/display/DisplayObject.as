//
// $Id: $

package flash.display {

import flash.events.EventDispatcher;

public class DisplayObject extends EventDispatcher
{
    public function DisplayObject ()
    {
        _loaderInfo = LoaderInfo.maybeCreateLoaderInfo();
    }

    public function get root () :DisplayObject
    {
        // in Thane world, there are no display object hierarchies
        return this;
    }

    public function get loaderInfo () :LoaderInfo
    {
        return _loaderInfo;
    }

    private var _loaderInfo :LoaderInfo;
}

}
