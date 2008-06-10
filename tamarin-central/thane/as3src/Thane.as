//
// $Id: $

package {

import flash.events.EventDispatcher;
import flash.utils.Dictionary;
import avmplus.Domain;

public class Thane
{
    public static function getBridgeToDomain (domainId :String) :EventDispatcher
    {
        return _bridges[domainId];
    }

    public static function spawnDomain (domainId :String, bridge :EventDispatcher) :Domain
    {
        var dom :Domain = new Domain();

        var thane :Class = dom.getClass("Thanette");
        if (thane == null) {
            throw new Error ("Could not locate Thanette in new Domain");
        }
        var initFun :Function = thane["initializeDomain"];
        if (initFun == null) {
            throw new Error("Could not locate initializeDomain() on foreign Thanette class");
        }
        initFun(domainId, bridge, requestHeartbeat);
        _bridges[domainId] = bridge;
        return dom;
    }

    public static function requestHeartbeat (heart :Function) :void
    {
        if (-1 == _hearts.indexOf(heart)) {
            _hearts.push(heart);
        }
    }

    public static function heartbeat () :void
    {
        for each (var heart :Function in _hearts) {
            try {
                heart();
            } catch (err :Error) {
                trace("Heartbeat error: " + err);
            }
        }
    }

    private static var _bridges :Dictionary = new Dictionary();
    private static var _hearts :Array = new Array();

    private static const _tracer :EventDispatcher = new EventDispatcher();
}
}
