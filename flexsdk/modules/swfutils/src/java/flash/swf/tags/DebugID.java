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

package flash.swf.tags;

import flash.swf.Tag;
import flash.swf.TagValues;
import flash.swf.types.FlashUUID;

public class DebugID extends Tag
{
	public DebugID(int code)
	{
		super(code);
	}

    public DebugID(FlashUUID uuid)
    {
        super(TagValues.stagDebugID);
        this.uuid = uuid;
    }

    public void visit(flash.swf.TagHandler h)
	{
		h.debugID(this);
	}

    public FlashUUID uuid;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DebugID))
        {
            DebugID debugID = (DebugID) object;

            if ( equals(debugID.uuid, this.uuid))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
