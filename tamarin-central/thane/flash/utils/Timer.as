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

    public function get currentCount () :int
    {
        return _currentCount;
    }

    public function get delay () :Number
    {
        return _delay;
    }

    public function get repeatCount () :int
    {
        return _repeatCount;
    }

    public function get running () :Boolean
    {
        return _buddy != null;
    }

    public function start () :void
    {
        if (_buddy != null) {
            return;
        }

        queueTimer(getTimer() + _delay);
    }

    public function stop () :void
    {
        // cut any current enqueued buddy adrift
        if (_buddy != null) {
            _buddy.budette = null;
            _buddy = null;
        }
    }

    public function reset () :void
    {
        stop();
        _currentCount = 0;
    }

    protected function expire (buddy :Buddy) :void
    {
        if (buddy != _buddy) {
            // the timer was stopped since this buddy was enqueued
            return;
        }
        _currentCount ++;
        dispatchEvent(new TimerEvent(TimerEvent.TIMER));
        if (_currentCount < _repeatCount) {
            queueTimer(buddy.expiration + _delay);

        } else {
            dispatchEvent(new TimerEvent(TimerEvent.TIMER_COMPLETE));
            _buddy = null;
        }
    }

    private function queueTimer (when :int) :void
    {
        // spawn a new buddy and toss it on the heap
        _buddy = new Buddy(this, when);
        if (_heap.enqueue(_buddy)) {
            return;
        }

        // TODO: formalize this as a quota rather than an implementation deficiency?
        throw new Error("Too many simultaneous running flash.utils.Timer objects");
    }

    protected static function heartbeat () :void
    {
        var now :int = getTimer();

        while (_heap.size > 0 && _heap.front.expiration <= now) {
            var buddy :Buddy = _heap.dequeue();
            buddy.budette.expire(buddy);
        }
    }

    private static function compareTimers (a :Buddy, b :Buddy) :int
    {
        // b-a rather than a-b so as to order low values before high
        return b.expiration - a.expiration;
    }

    private var _currentCount :int;
    private var _delay :Number;
    private var _repeatCount :int;
    private var _buddy :Buddy;

    private static var _heap :Heap;
}
}

import flash.utils.Timer;

class Buddy {
    public var budette :Timer;
    public var expiration :int;

    public function Buddy (timer :Timer, expiration :int)
    {
        this.budette = timer;
        this.expiration = expiration;
    }
}
