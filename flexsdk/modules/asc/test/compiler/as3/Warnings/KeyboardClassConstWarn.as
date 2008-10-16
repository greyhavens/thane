/*
*Coach mode warning for key class constants
*/

package Warnings{

    import flash.ui.Keyboard;
    import flash.display.*;

    public class KeyboardExample extends Sprite{   
        var KB:Keyboard = new Keyboard();

        Keyboard.CAPSLOCK;
        Keyboard.DELETEKEY;
        Keyboard._listeners;
        Keyboard.PGDN;
        Keyboard.PGUP;
    }
    
}
