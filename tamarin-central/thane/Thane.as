//
// $Id: $

package {

import flash.events.EventDispatcher;

public class Thane
{
    public static function getTracer () :EventDispatcher
    {
        return _tracer;
    }

    public static function getDomainId () :String
    {
        return _domainId;
    }

    public static function getDomainBridge () :EventDispatcher
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
            foreignHeart(heartbeat);
        }
    }

    public static function requestHeartbeat (heart :Function) :void
    {
        if (-1 == _hearts.indexOf(heart)) {
            _hearts.push(heart);
        }
    }

    private static function heartbeat () :void
    {
        for each (var heart :Function in _hearts) {
            try {
                heart();
            } catch (err :Error) {
                trace("Heartbeat error: " + err);
            }
        }
    }

    private static var _initialized :Boolean;
    private static var _domainId :String = "System"; // TODO: distinguish from anonymous domains?
    private static var _bridge :EventDispatcher;
    private static var _hearts :Array = new Array();

    private static const _tracer :EventDispatcher = new EventDispatcher();
}
}
