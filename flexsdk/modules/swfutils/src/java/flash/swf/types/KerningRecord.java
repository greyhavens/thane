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
public class KerningRecord
{
    public int code1;
	public int code2;
	public int adjustment;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof KerningRecord)
        {
            KerningRecord kerningRecord = (KerningRecord) object;

            if ( (kerningRecord.code1 == this.code1) &&
                 (kerningRecord.code2 == this.code2) &&
                 (kerningRecord.adjustment == this.adjustment) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
