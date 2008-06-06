//
// $Id: $

package {

import flash.events.EventDispatcher;

public class Thanette
{
    public static function getDomainId () :String
    {
        return _domainId;
    }

    public static function getBridgeHome () :EventDispatcher
    {
        return _bridge;
    }

    public static function initializeDomain (
        domainId :String, bridge :EventDispatcher, foreignHeart :Function) :void
    {
        if (_initialized) {
            throw new Error("This domain has already been initialized");
        }
        _initialized = true;
        _domainId = domainId;
        _bridge = bridge;

        if (foreignHeart != null) {
            foreignHeart(Thane.heartbeat);
        }
    }

    public static function trace (s :Array) :void
    {
        if (_bridge != null) {
            _bridge.dispatchEvent(new TraceEvent(TraceEvent.TRACE, false, false, s));
        }
    }

    private static var _initialized :Boolean;
    private static var _domainId :String;
    private static var _bridge :EventDispatcher;
}
}
