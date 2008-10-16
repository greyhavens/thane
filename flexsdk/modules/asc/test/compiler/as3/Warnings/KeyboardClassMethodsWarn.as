/*******************************************************************************************
*******Coach Warning for the methods of Key Class which are changed/removed in AS3**********
*******************************************************************************************/

package Warnings{
    import flash.ui.Keyboard;
    
    var listener:* = new Object();

    Keyboard.addListener(listener);
    Keyboard.getAscii();
    Keyboard.getCode();
    Keyboard.isDown();
    Keyboard.isToggled();
    Keyboard.removeListener();
    Keyboard.onKeyDown = function():void{}
    Keyboard.onKeyUp = function():void{}
   
    

}