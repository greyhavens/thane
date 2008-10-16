////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf;

import flash.swf.types.Rect;

/**
 * @author Clement Wong
 */
public class Header
{
	public Header()
	{
	}

	public static boolean useCompression(int version)
	{
		if ((System.getProperty("flex.swf.uncompressed") != null))
			return false;

		return version >= 6;
	}

    public boolean compressed;
	public int version;
	public long length;
	public Rect size;
	public int rate;
	public int framecount;
}
