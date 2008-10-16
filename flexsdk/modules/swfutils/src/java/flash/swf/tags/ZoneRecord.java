////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import java.util.Arrays;

/**
 * @author Brian Deitte
 */
public class ZoneRecord
{
    public boolean equals(Object object)
    {
        boolean isEqual = false;
        if (super.equals(object) &&  (object instanceof ZoneRecord))
        {
            ZoneRecord record = (ZoneRecord)object;
            if (numZoneData == record.numZoneData &&
                Arrays.equals(zoneData, record.zoneData) &&
                zoneMask == record.zoneMask)
            {
                isEqual = true;
            }
        }
        return isEqual;
    }

    public int numZoneData;
    public long[] zoneData;
    public int zoneMask;
}
