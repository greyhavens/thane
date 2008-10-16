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
public class GradRecord
{
	public GradRecord()
	{
	}

	public GradRecord(int r, int c)
	{
		ratio = r;
		color = c;
	}

	public int ratio;

    /** color as int: 0xAARRGGBB or 0x00RRGGBB */
	public int color;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof GradRecord)
        {
            GradRecord gradRecord = (GradRecord) object;

            if ( (gradRecord.ratio == this.ratio) &&
                 (gradRecord.color == this.color) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
