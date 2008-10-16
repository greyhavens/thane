////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.graphics.images;

/**
 * @author Peter Farland
 */
public abstract class BitmapImage
{
	protected BitmapImage()
	{
	}

	public int getWidth()
	{
		return width;
	}

	public int getHeight()
	{
		return height;
	}

	public long getModified()
	{
		return modified;
	}

	public String getLocation()
	{
		return location;
	}

	public abstract void dispose();

	protected int width;
	protected int height;
	protected String location;
	protected long modified;
}
