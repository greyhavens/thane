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

import java.util.Arrays;

/**
 * @author Clement Wong
 */
public class SoundInfo
{
	public static final int UNINITIALIZED = -1;

	public boolean syncStop;
	public boolean syncNoMultiple;

	// they are unsigned, so if they're -1, they're not initialized.
	public long inPoint = UNINITIALIZED;
	public long outPoint = UNINITIALIZED;
	public int loopCount = UNINITIALIZED;

    /** pos44:32, leftLevel:16, rightLevel:16 */
	public long[] records = new long[0];

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof SoundInfo)
        {
            SoundInfo soundInfo = (SoundInfo) object;

            if ( (soundInfo.syncStop == this.syncStop) &&
                 (soundInfo.syncNoMultiple == this.syncNoMultiple) &&
                 (soundInfo.inPoint == this.inPoint) &&
                 (soundInfo.outPoint == this.outPoint) &&
                 (soundInfo.loopCount == this.loopCount) &&
                 Arrays.equals(soundInfo.records, this.records) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
