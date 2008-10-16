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
public interface ShapeIterator
{
	public boolean isDone();
	public short currentSegment(double[] coords);
	public void next();

	public static final short MOVE_TO = 0;
	public static final short LINE_TO = 1;
	public static final short QUAD_TO = 2;
	public static final short CUBIC_TO = 3;
	public static final short CLOSE = 4;
}
