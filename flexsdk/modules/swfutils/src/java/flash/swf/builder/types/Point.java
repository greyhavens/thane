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

package flash.swf.builder.types;

/**
 * @author Peter Farland
 */
public class Point
{
	public Point()
	{
		x = 0.0;
		y = 0.0;
	}

	public Point(double x, double y)
	{
		this.x = x;
		this.y = y;
	}

	public double x;
	public double y;
}
