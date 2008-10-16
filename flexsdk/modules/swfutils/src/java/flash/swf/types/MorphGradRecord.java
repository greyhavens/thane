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

/**
 * @author Clement Wong
 */
public class MorphGradRecord
{
    public int startRatio;
    public int endRatio;

    /** colors as ints: 0xAARRGGBB */
	public int startColor;
	public int endColor;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof MorphGradRecord)
        {
            MorphGradRecord morphGradRecord = (MorphGradRecord) object;

            if ( (morphGradRecord.startRatio == this.startRatio) &&
                 (morphGradRecord.startColor == this.startColor) &&
                 (morphGradRecord.endRatio == this.endRatio) &&
                 (morphGradRecord.endColor == this.endColor) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }    
}
