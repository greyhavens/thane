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

package flash.swf.types;

import flash.swf.SwfEncoder;
import flash.swf.tags.DefineTag;

/**
 * @author Clement Wong
 */
public class Rect
{
    public Rect(int width, int height)
    {
        xMax = width;
        yMax = height;
        nbits();
    }

    public Rect()
    {
    }

    public Rect(int xMin, int xMax, int yMin, int yMax)
    {
        this.xMin = xMin;
        this.xMax = xMax;
        this.yMin = yMin;
        this.yMax = yMax;
    }

	public int xMin;
	public int xMax;
	public int yMin;
	public int yMax;

	public String toString()
	{
        if ((xMin != 0) || (yMin != 0))
        {
            return "(" + xMin + "," + yMin + "),(" + xMax + "," + yMax + ")";
        }
        else
        {
		    return new StringBuilder().append(xMax).append('x').append(yMax).toString();
        }
	}

    public int nbits()
    {
        int maxCoord = SwfEncoder.maxNum(xMin, xMax, yMin, yMax);
        return SwfEncoder.minBits(maxCoord,1);
    }

    public int getWidth()
    {
        return xMax-xMin;
    }

    public int getHeight()
    {
        return yMax-yMin;
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof Rect)
        {
            Rect rect = (Rect) object;

            if ( (rect.xMin == this.xMin) &&
                 (rect.xMax == this.xMax) &&
                 (rect.yMin == this.yMin) &&
                 (rect.yMax == this.yMax) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int hashCode() {
      int hashCode = super.hashCode();
      hashCode += DefineTag.PRIME * xMin;
      hashCode += DefineTag.PRIME * xMax;
      hashCode += DefineTag.PRIME * yMin;
      hashCode += DefineTag.PRIME * yMax;

      return hashCode;
    }

}
