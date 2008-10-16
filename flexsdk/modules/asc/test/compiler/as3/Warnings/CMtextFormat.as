/**
 *	
 *  	Coach Mode for flash.display.TextFormat
 */

package Warnings {

import flash.text.TextFormat;
import flash.util.trace;

var tft:TextFormat = new TextFormat()
var metrics:*=tft.getTextExtent("theText"); 
trace(metrics)


}