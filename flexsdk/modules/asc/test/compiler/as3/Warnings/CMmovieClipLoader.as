/**
 *	
 *  	Coach Mode for MovieClipLoader
 */

package Warnings {
	
	import flash.display.Loader;
	import flash.display.MovieClip;

	var container:MovieClip = new MovieClip();
	var mcLoader:Loader=new Loader();
	var mclListener:Object = new Object();

	mcLoader.addListener(mclListener);
        mcLoader.getProgress(container);
	mcLoader.loadClip("YourImage.jpg", container);
        mcLoader.removeListener(mclListener);
        mcLoader.onLoadComplete=function():void{
            trace("on load complete");
        }
        mcLoader.onLoadError=function():void{
            trace("On load error");
        }
        mcLoader.onLoadInit= function():void{
            trace("on load init");
        }
        mcLoader.onLoadProgress = function():void{
            trace("on load progress");
        }
        mcLoader.onLoadStart= function():void{
            trace("on load start");
        }
		
}