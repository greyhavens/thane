/******************************************************************************************************************Coach Warning for Mouse Class****************************************
*******************************************************************************************/

package Warnings{
     import flash.ui.*;
     import flash.util.*;

     var listener:Object = new Object();
     Mouse.addListener(listener);
     
     var ishidden:Number = Mouse.hide();
     trace(ishidden);

     var washidden:Number=Mouse.show();
     trace(washidden);

     Mouse.removeListener(listener);

     var someListener:Object = new Object();
     someListener.onMouseDown = function():void{
         trace("On Mouse Down");
     };
     someListener.onMouseMove = function():void{
         trace("on Mouse Move");
     };
     someListener.onMouseUp = function():void{
         trace("On Mouse Up");
     };
     someListener.onMouseWheel = function():void{
         trace("on Mouse wheel");
     }


    
}