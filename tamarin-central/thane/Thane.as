//
// $Id: $

package {

public class Thane
{
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
                // TODO: how are we doing logging?
                trace("Heartbeat error: " + err);
            }
        }
    }

    private static var _hearts :Array = new Array();
}
}
