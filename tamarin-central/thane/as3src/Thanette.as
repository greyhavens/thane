//
// $Id: $

package {

import flash.events.EventDispatcher;

public class Thanette
{
    public static function isSystemDomain () :Boolean
    {
        return _domainId == null;
    }

    public static function getDomainId () :String
    {
        return _domainId;
    }

    public static function getBridgeHome () :EventDispatcher
    {
        return _bridge;
    }

    public static function getConsoleTracePrefix () :String
    {
        return _consoleTracePrefix;
    }

    public static function initializeDomain (
        domainId :String, consoleTracePrefix :String, bridge :EventDispatcher,
        foreignHeart :Function) :void
    {
        if (domainId == null || domainId.length == 0) {
            throw new Error ("Domain must be spawned with an identifier");
        }
        if (_initialized) {
            throw new Error("This domain has already been initialized");
        }
        _initialized = true;
        _domainId = domainId;
        _bridge = bridge;
        _consoleTracePrefix = consoleTracePrefix;

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
    private static var _consoleTracePrefix :String;
}
}
