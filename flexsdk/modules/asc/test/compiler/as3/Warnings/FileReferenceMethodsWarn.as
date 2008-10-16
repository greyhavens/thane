/*************************************************************************************************************Coach Warning for Methods of FileReference Class**************************
*******************************************************************************************/

package warnings{
    import flash.net.FileReference;

    var listener:* = new Object();


    var fileRef:FileReference = new FileReference();
    fileRef.addListener(listener);
    fileRef.removeListener(listener);

}
