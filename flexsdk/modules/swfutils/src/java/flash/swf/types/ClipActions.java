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

import java.util.List;

/**
 * @author Clement Wong
 */
public class ClipActions
{
    /**
     * All events used in these clip actions
     */
	public int allEventFlags;

    /**
     * Individual event handlers.  List of ClipActionRecord instances.
     */
	public List clipActionRecords;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof ClipActions)
        {
            ClipActions clipActions = (ClipActions) object;

            if ( (clipActions.allEventFlags == this.allEventFlags) &&
                 ( ( (clipActions.clipActionRecords == null) && (this.clipActionRecords == null) ) ||
                   ( (clipActions.clipActionRecords != null) && (this.clipActionRecords != null) &&
                     ArrayLists.equals( clipActions.clipActionRecords,
                                    this.clipActionRecords ) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
