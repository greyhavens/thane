package com.whirled.game.server {

import flash.display.DisplayObject;

public class ServerObject extends DisplayObject
{
}

}

package zell.coolgame {

import com.whirled.game.server.ServerObject;

public class MyCoolGameServer extends ServerObject
{
    public function MyCoolGameServer ()
    {
        _ctrl = new TestControl(this);

        trace("GameControl.isConnected() = " + _ctrl.isConnected());
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
