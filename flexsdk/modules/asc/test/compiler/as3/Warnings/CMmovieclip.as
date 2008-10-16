/**
 *	
 *  	Coach Mode for flash.display.MovieClip
 */

package Warnings {

	import flash.display.MovieClip;
	
	
	var a_mc:MovieClip = new MovieClip();
	
	duplicateMovieClip(a_mc);
	a_mc.duplicateMovieClip();
	a_mc.createEmptyMovieClip("the_mc",8);
	a_mc.swapDepths(7)
	a_mc.loadVariables("test.txt");
	a_mc.attachMovie()
	a_mc.beginBitmapFill()
	a_mc.beginFill()
	a_mc.beginGradientFill()
	a_mc.clear()
	a_mc.createTextField()
	a_mc.curveTo()
	a_mc.endFill()
	a_mc.getBytesLoaded()
	a_mc.getDepth()
	a_mc.getInstanceAtDepth()
	a_mc.getNextHighestDepth()
	a_mc.getTextSnapshot()
	a_mc.getURL()
	a_mc.hitTest()
	a_mc.lineGradientStyle()
	a_mc.lineStyle()
	a_mc.lineTo()
	a_mc.loadMovie()
	a_mc.removeMovieClip()
	a_mc.setMask()
	a_mc.unloadMovie()
	a_mc.attachAudio();
	a_mc.attachBitmap();
	a_mc.getBytesTotal()
	a_mc.getSWFVersion()

	trace(a_mc._alpha);
	trace(a_mc._x); // warning, _x is no longer supported.  Use x instead.
	a_mc._rotation = 10;
	a_mc._focusrect
	a_mc._lockroot = true;
	a_mc._highquality = false;
	a_mc._name = true;
	a_mc._currentframe
	a_mc._droptarget
	a_mc._framesloaded
	a_mc._height
	a_mc._parent
	a_mc._quality
	a_mc._soundbuftime
	a_mc._target
	a_mc._totalframes
	a_mc._url
	a_mc._visible
	a_mc._width
	a_mc._xmouse
	a_mc._xscale
	a_mc._y
	a_mc._ymouse
	a_mc._yscale

	a_mc.focusEnabled = true;
	
		
}
