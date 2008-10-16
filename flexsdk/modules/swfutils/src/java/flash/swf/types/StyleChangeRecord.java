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

import flash.swf.SwfEncoder;
import flash.swf.Tag;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * @author Clement Wong
 */
public class StyleChangeRecord extends ShapeRecord
{
	public boolean stateNewStyles;
	public boolean stateLineStyle;
	public boolean stateFillStyle1;
	public boolean stateFillStyle0;
	public boolean stateMoveTo;

	public int moveDeltaX;
	public int moveDeltaY;

	public int fillstyle0;
    /**
     * This is an index into the fillstyles array starting with 1.
     */
	public int fillstyle1;
	public int linestyle;

	public ArrayList fillstyles;
	public ArrayList linestyles;

    public StyleChangeRecord()
    {
    }

    public StyleChangeRecord(boolean stateFillStyle1, int fillstyle1, boolean stateMoveTo,
                             int moveDeltaX, int moveDeltaY)
    {
        this.stateFillStyle1 = stateFillStyle1;
        this.fillstyle1 = fillstyle1;
        this.stateMoveTo = stateMoveTo;
        this.moveDeltaX = moveDeltaX;
        this.moveDeltaY = moveDeltaY;
    }

    public String toString()
	{
		String retVal = "Style: newStyle=" + stateNewStyles + " lineStyle=" + stateLineStyle + " fillStyle=" + stateFillStyle1 + 
			" fileStyle0=" + stateFillStyle0 + " moveTo=" + stateMoveTo;
		
		if (stateMoveTo)
		{
			retVal += " x=" + moveDeltaX + " y=" + moveDeltaY;
		}
		
		return retVal;
	}
    
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if ( super.equals(object) && (object instanceof StyleChangeRecord) )
        {
            StyleChangeRecord styleChangeRecord = (StyleChangeRecord) object;

            if ( (styleChangeRecord.stateNewStyles == this.stateNewStyles) &&
                 (styleChangeRecord.stateLineStyle == this.stateLineStyle) &&
                 (styleChangeRecord.stateFillStyle1 == this.stateFillStyle1) &&
                 (styleChangeRecord.stateFillStyle0 == this.stateFillStyle0) &&
                 (styleChangeRecord.stateMoveTo == this.stateMoveTo) &&
                 (styleChangeRecord.moveDeltaX == this.moveDeltaX) &&
                 (styleChangeRecord.moveDeltaY == this.moveDeltaY) &&
                 (styleChangeRecord.fillstyle0 == this.fillstyle0) &&
                 (styleChangeRecord.fillstyle1 == this.fillstyle1) &&
                 (styleChangeRecord.linestyle == this.linestyle) &&
                 ( ( (styleChangeRecord.fillstyles == null) && (this.fillstyles == null) ) ||
                   ( (styleChangeRecord.fillstyles != null) && (this.fillstyles != null) &&
                     ArrayLists.equals( styleChangeRecord.fillstyles,
                                    this.fillstyles ) ) ) &&
                 ( ( (styleChangeRecord.linestyles == null) && (this.linestyles == null) ) ||
                   ( (styleChangeRecord.linestyles != null) && (this.linestyles != null) &&
                     ArrayLists.equals( styleChangeRecord.linestyles,
                                    this.linestyles ) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public void setMove(int x, int y)
    {
        stateMoveTo = true;
        moveDeltaX = x;
        moveDeltaY = y;
    }

	public void getReferenceList( List<Tag> list )
    {
        if (fillstyles != null)
        {
            Iterator it = fillstyles.iterator();
            while (it.hasNext())
            {
                FillStyle style = (FillStyle) it.next();
                if (style.hasBitmapId() && style.bitmap != null)
                    list.add( style.bitmap );
            }
        }

    }

    public int nMoveBits()
    {
        return SwfEncoder.minBits(SwfEncoder.maxNum(moveDeltaX, moveDeltaY, 0, 0), 1);
    }

    public void setFillStyle1(int index)
    {
        stateFillStyle1 = true;
        fillstyle1 = index;
    }

	public void setFillStyle0(int index)
	{
		stateFillStyle0 = true;
		fillstyle0 = index;
	}

    public void setLinestyle(int index)
    {
        stateLineStyle = true;
        linestyle = index;
    }

	public void addToDelta(int x, int y)
	{
		moveDeltaX += x;
		moveDeltaY += y;
	}
}
