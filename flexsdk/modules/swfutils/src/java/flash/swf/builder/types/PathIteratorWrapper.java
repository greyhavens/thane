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

import java.awt.geom.PathIterator;

/**
 * @author Peter Farland
 */
public class PathIteratorWrapper implements ShapeIterator
{
	private PathIterator pi;

	public PathIteratorWrapper(PathIterator pi)
	{
		this.pi = pi;
	}

	public short currentSegment(double[] coords)
	{
		int code = pi.currentSegment(coords);
		return (short) code;
	}

	public boolean isDone()
	{
		return pi.isDone();
	}

	public void next()
	{
		pi.next();
	}

}
