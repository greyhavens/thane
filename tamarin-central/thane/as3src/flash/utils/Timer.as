//
// $Id: $

package flash.utils {

import flash.events.EventDispatcher;
import flash.events.TimerEvent;

import de.polygonal.ds.Heap;

public class Timer extends EventDispatcher
{
    public function Timer (delay :Number, repeatCount :int = 0)
    {
        if (_heap == null) {
            _heap = new Heap(10000, compareTimers);
            Thane.requestHeartbeat(heartbeat);
        }

        _delay = delay;
        _repeatCount = repeatCount;
        reset();
    }

    /** The total number of times the timer has fired since it started at zero. */
    public function get currentCount () :int
    {
        return _currentCount;
    }

    /** The delay, in milliseconds, between timer events. */
    public function get delay () :Number
    {
        return _delay;
    }

    /** The delay, in milliseconds, between timer events. */
    public function set delay (value :Number) :void
    {
        _delay = value;
        if (running) {
            stop();
            start();
        }
    }
 
    /** The total number of times the timer is set to run. */
    public function get repeatCount () :int
    {
        return _repeatCount;
    }

    /** The total number of times the timer is set to run. */
    public function set repeatCount (count :int) :void
    {
        _repeatCount = count;
        if (_repeatCount > 0 && _repeatCount < _currentCount) {
            stop();
        }
    }

    /** The timer's current state; true if the timer is running, otherwise false. */
    public function get running () :Boolean
    {
        return _buddy != null;
    }

    /**
     * Starts the timer, if it is not already running.
     */
    public function start () :void
    {
        if (_buddy != null) {
            return;
        }

        scheduleBuddy(new Buddy(this));
    }

    /**
     * Stops the timer.
     */
    public function stop () :void
    {
        // cut any current enqueued buddy adrift
        if (_buddy != null) {
            _buddy.budette = null;
            _buddy = null;
        }
    }

    /**
     * Stops the timer, if it is running, and sets the currentCount property back to 0,
     * like the reset button of a stopwatch.
     */
    public function reset () :void
    {
        stop();
        _currentCount = 0;
    }

    protected function expire (buddy :Buddy) :void
    {
        if (buddy !== _buddy) {
            // the timer was stopped since this buddy was enqueued
            return;
        }

        _currentCount ++;
        dispatchEvent(new TimerEvent(TimerEvent.TIMER));

        // if the timer was stop()'ed in the TIMER event itself, do not requeue!
        if (_buddy == null) {
            return;
        }

        // if we're repeating forever, or still got a ways to go, reschedule, else finish
        if (repeatCount == 0 || _currentCount < _repeatCount) {
            scheduleBuddy(_buddy);

        } else {
            dispatchEvent(new TimerEvent(TimerEvent.TIMER_COMPLETE));
            _buddy = null;
        }
    }

    private function scheduleBuddy (buddy :Buddy) :void
    {
        _buddy = buddy;
        _buddy.expiration = getTimer() + Math.max(_delay, 5);
        if (_heap.enqueue(_buddy)) {
            return;
        }

        // TODO: formalize this as a quota rather than an implementation deficiency?
        throw new Error("Too many simultaneous running flash.utils.Timer objects");
    }

    private static function compareTimers (a :Buddy, b :Buddy) :int
    {
        // b-a rather than a-b so as to order low values before high
        return b.expiration - a.expiration;
    }

    protected static function heartbeat () :void
    {
        var now :int = getTimer();

        // see if we should pop one timer off the queue and expire it; we could pop all the
        // outstanding ones, but this solution puts the onus on developers to make their event
        // handlers light-weight, and protects the socket code from starvation
        if (_heap.size > 0 && _heap.front.expiration <= now) {
            var buddy :Buddy = _heap.dequeue();
            if (buddy.budette != null) {
                buddy.budette.expire(buddy);
            }
        }
    }

    static function createTimer (closure :Function, delay :Number,
                                 timeout :Boolean, args :Array) :uint
    {
        var timer :Timer = new Timer(delay, timeout ? 1 : 0);
        var thisIx = _timerIx;
        var fun :Function = function (event :TimerEvent) :void {
            if (timeout) {
                destroyTimer(thisIx);
            }
            closure.apply(null, args);
        };
        timer.addEventListener(TimerEvent.TIMER, fun);
        _timers[_timerIx] = [ timer, fun ];
        timer.start();
        return _timerIx ++;
    }

    static function destroyTimer (id :uint) :void
    {
        var bits :Array = _timers[id];
        if (bits) {
            bits[0].removeEventListener(TimerEvent.TIMER, bits[1]);
            bits[0].stop();
            delete _timers[id];
        }
    }

    private var _currentCount :int;
    private var _delay :Number;
    private var _repeatCount :int;
    private var _buddy :Buddy;

    private static var _heap :Heap;
    private static var _timers :Dictionary = new Dictionary();
    private static var _timerIx :uint = 1;
}
}

import flash.utils.Timer;
import flash.utils.getTimer;

class Buddy {
    public var budette :Timer;
    public var expiration :int;

    public function Buddy (timer :Timer)
    {
        this.budette = timer;
    }
}
