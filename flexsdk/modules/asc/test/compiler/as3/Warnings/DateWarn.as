/************************************************************************************************getUTCYear(),getYear() and setYear() should throw a warning that method was removed****
*****and so Use Date.getUTCFullYear(),Date.getFullYear(),Date.setFullYear() instead     ****
*******************************************************************************************/





package warnings{
   var date:Date = new Date(0);
   trace(date.getUTCYear());
   trace(date.getYear());
   date.setYear(1969);
   trace(date.getFullYear());
}