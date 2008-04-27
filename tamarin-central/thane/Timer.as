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
        _currentCount = 0;
        _expiration = 0;
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
        return _expiration > 0;
    }

    protected function get nextExpirationStamp() :int
    {
        return _expiration;
    }

    public function start () :void
    {
        if (_expiration > 0) {
            return;
        }

        _currentCount = 0;
        queueTimer(getTimer() + _delay);
    }

    public function stop () :void
    {
        // TODO: Find or write a free Heap implementation with a full Remove operation
        throw new Error("Not implemented");
    }

    public function reset () :void
    {
        // TODO: Find or write a free Heap implementation with a full Remove operation
        throw new Error("Not implemented");
    }

    protected function expire () :void
    {
        _currentCount ++;
        dispatchEvent(new TimerEvent(TimerEvent.TIMER));
        if (_currentCount < _repeatCount) {
            queueTimer(_expiration + _delay);

        } else {
            dispatchEvent(new TimerEvent(TimerEvent.TIMER_COMPLETE));
            _expiration = 0;
        }
    }

    private function queueTimer (when :int) :void
    {
        _expiration = when;
        if (_heap.enqueue(this)) {
            return;
        }

        // TODO: formalize this as a quota rather than an implementation deficiency?
        throw new Error("Too many simultaneous running flash.utils.Timer objects");
    }

    protected static function heartbeat () :void
    {
        var now :int = getTimer();

        while (_heap.size > 0 && _heap.front.nextExpirationStamp <= now) {
            _heap.dequeue().expire();
        }
    }

    private static function compareTimers (a :Timer, b :Timer) :int
    {
        // b-a rather than a-b so as to order low values before high
        return b.nextExpirationStamp - a.nextExpirationStamp;
    }

    private var _currentCount :int;
    private var _delay :Number;
    private var _repeatCount :int;
    private var _expiration :int;

    private static var _heap :Heap;
}
}
