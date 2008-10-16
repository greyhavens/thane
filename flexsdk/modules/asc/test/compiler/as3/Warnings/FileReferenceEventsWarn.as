/*******************************************************************************************
*************Coach mode warning for the eventhandlers of FileReference Class****************
*******************************************************************************************/

package warnings{

    import flash.net.FileReference;

    var fileRef:FileReference = new FileReference();
 
    fileRef.onCancel = function():void{}
    fileRef.onComplete = function():void{}
    fileRef.onHTTPError = function():void{}
    fileRef.onIOError = function():void{}
    fileRef.onOpen = function():void{}
    fileRef.onProgress = function():void{}
    fileRef.onSecurityError = function():void{}
    fileRef.onSelect = function():void{}

}