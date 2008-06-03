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

        var timer :Timer = new Timer(3);
        timer.addEventListener(TimerEvent.TIMER, function (evt :TimerEvent) :void {
            trace("Timer expired!");
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
