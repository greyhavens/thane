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
public class CurvedEdgeRecord extends EdgeRecord
{
    public int controlDeltaX;
	public int controlDeltaY;
	public int anchorDeltaX;
	public int anchorDeltaY;

	public void addToDelta(int xTwips, int yTwips)
	{
		anchorDeltaX += xTwips;
		anchorDeltaY += yTwips;
	}
	
	public String toString()
	{
		return "Curve: cx=" + controlDeltaX + " cy=" + controlDeltaY + " dx=" + anchorDeltaX + " dy=" + anchorDeltaY;
	}

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof CurvedEdgeRecord))
        {
            CurvedEdgeRecord curvedEdgeRecord = (CurvedEdgeRecord) object;

            if ( (curvedEdgeRecord.controlDeltaX == this.controlDeltaX) &&
                 (curvedEdgeRecord.controlDeltaY == this.controlDeltaY) &&
                 (curvedEdgeRecord.anchorDeltaX == this.anchorDeltaX) &&
                 (curvedEdgeRecord.anchorDeltaY == this.anchorDeltaY) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
