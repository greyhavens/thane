/**
 *	
 *  	Coach Mode for flash.xml.XMLDocument
 */

package Warnings {

import flash.xml.XMLDocument;
import flash.util.*;

var xmldoc:XMLDocument = new XMLDocument("<mx>content</mx>")
 
trace(xmldoc.contentType);
xmldoc.loaded;
xmldoc.status;
xmldoc.addRequestHeader();
xmldoc.getBytesLoaded();
xmldoc.getBytesTotal();
xmldoc.load();
xmldoc.send();
xmldoc.sendAndLoad();
xmldoc.onData = function():void{
	//do nothing
};
xmldoc.onHTTPStatus = function():void{
};
xmldoc.onLoad = function():void{
};

}