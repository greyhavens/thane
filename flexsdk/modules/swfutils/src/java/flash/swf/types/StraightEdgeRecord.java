////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.types;

/**
 * @author Clement Wong
 */
public class StraightEdgeRecord extends EdgeRecord
{
	public int deltaX = 0;
	public int deltaY = 0;

    public StraightEdgeRecord setX(int x)
    {
        deltaX = x;
        return this;
    }
    
    public StraightEdgeRecord setY(int y)
    {
        deltaY = y;
        return this;
    }

    public StraightEdgeRecord()
    {
    }
    
    public StraightEdgeRecord(int deltaX, int deltaY)
    {
        this.deltaX = deltaX;
        this.deltaY = deltaY;
    }

	public void addToDelta(int xTwips, int yTwips)
	{
		deltaX += xTwips;
		deltaY += yTwips;
	}
	public String toString()
	{
		return "Line: x=" + deltaX + " y=" + deltaY;
	}

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof StraightEdgeRecord)
        {
            StraightEdgeRecord straightEdgeRecord = (StraightEdgeRecord) object;

            if ( (straightEdgeRecord.deltaX == this.deltaX) &&
                 (straightEdgeRecord.deltaY == this.deltaY) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
