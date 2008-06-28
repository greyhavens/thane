//
// $Id: $

package flash.utils {

import flash.events.TimerEvent;
import avmplus.System;

public function setTimeout (closure :Function, delay :Number, ... arguments) :uint
{
    return Timer.createTimer(closure, delay, true, arguments);
}

public function setInterval (closure :Function, delay :Number, ... arguments) :uint
{
    return Timer.createTimer(closure, delay, false, arguments);
}

public function clearTimeout (id :uint): void
{
    Timer.destroyTimer(id);
}

public function clearInterval (id :uint): void
{
    Timer.destroyTimer(id);
}

public function getTimer () :int
{
    return System.getTimer();
}
}
