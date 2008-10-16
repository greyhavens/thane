/*******************************************************************************************
********Coach mode Warning for FileReferenceList Class methods******************************
*******************************************************************************************/

package Warnings{
    import flash.net.FileReferenceList;
   
    var fileRefList:FileReferenceList = new FileReferenceList();

    fileRefList.addListener();
    fileRefList.removeListener();
    fileRefList.onCancel = function():void{}
    fileRefList.onSelect = function():void{}

}