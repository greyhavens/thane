/*******************************************************************************************
********************Coach Mode Warnings for the methods of IME class************************
*******************************************************************************************/

package warnings{

    import flash.system.IME;

    var IMEvar:IME = new IME();

    IME.addListener();
    IME.getConversionMode();
    IME.getEnabled();
    IME.removeListener();
    IME.setConversionMode()
    IME.setEnabled();
    IME.onIMEComposition = function():void{}

}