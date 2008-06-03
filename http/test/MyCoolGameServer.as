package com.whirled.game.server {

import flash.display.DisplayObject;

public class ServerObject extends DisplayObject
{
}

}

package zell.coolgame {

import com.whirled.game.server.ServerObject;

import flash.events.TimerEvent;
import flash.utils.Timer;

public class MyCoolGameServer extends ServerObject
{
    public function MyCoolGameServer ()
    {
        _ctrl = new TestControl(this);

        trace("GameControl.isConnected() = " + _ctrl.isConnected());

        var timer :Timer = new Timer(1, 1);
        timer.addEventListener(TimerEvent.TIMER, function (evt :TimerEvent) :void {
            trace("Timer expired! Testing call trace...");
            throw new Error("Everything is working normally. There should be a stack trace here.");
        });
        timer.start();
    }

    protected var _ctrl :TestControl;
}

}

import flash.display.DisplayObject;

import com.whirled.AbstractControl;

class TestControl extends AbstractControl
{
    public function TestControl (disp :DisplayObject)
    {
        super(disp);
    }
}
