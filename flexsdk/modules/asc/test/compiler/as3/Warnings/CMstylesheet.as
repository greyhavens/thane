/**
 *	
 *  	Coach Mode for flash.text.StyleSheet
 */

package Warnings {

	import flash.text.StyleSheet;
        import flash.util.trace;
	
	var styl:StyleSheet = new StyleSheet()
	
	trace(styl)
	
	
	styl.getStyleNames()
	styl.load()
	styl.onLoad = function():void{}
	

}