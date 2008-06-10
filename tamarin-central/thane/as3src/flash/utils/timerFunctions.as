//
// $Id: $

package flash.utils {

import flash.events.TimerEvent;
import avmplus.System;

var timers :Dictionary = new Dictionary();
var timerIx :uint = 1;

public function setTimeout (closure :Function, delay :Number, ... arguments) :uint
{
    return createTimer(closure, delay, true, arguments);
}

public function setInterval (closure :Function, delay :Number, ... arguments) :uint
{
    return createTimer(closure, delay, false, arguments);
}

public function clearTimeout (id :uint): void
{
    destroyTimer(id);
}

public function clearInterval (id :uint): void
{
    destroyTimer(id);
}

public function getTimer () :int
{
    return System.getTimer();
}

function createTimer (closure :Function, delay :Number, timeout :Boolean, ... arguments) :uint
{
    var timer :Timer = new Timer(delay, timeout ? 1 : 0);
    var fun :Function = function (event :TimerEvent) :void {
        if (timeout) {
            destroyTimer(timerIx);
        }
        closure.apply(null, arguments);
    };
    timer.addEventListener(TimerEvent.TIMER, fun);
    timers[timerIx] = [ timer, fun ];
    return timerIx ++;
}

function destroyTimer (id :uint) :void
{
    var bits :Array = timers[id];
    if (bits) {
        bits[0].removeEventListener(TimerEvent.TIMER, bits[1]);
        bits[0].stop();
        delete timers[id];
    }
}

}
