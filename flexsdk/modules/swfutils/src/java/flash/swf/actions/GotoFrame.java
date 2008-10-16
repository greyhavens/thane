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

package flash.swf.actions;

import flash.swf.Action;
import flash.swf.ActionHandler;
import flash.swf.ActionConstants;

/**
 * @author Clement Wong
 */
public class GotoFrame extends Action
{
	public GotoFrame()
	{
		super(ActionConstants.sactionGotoFrame);
	}

    public void visit(ActionHandler h)
	{
		h.gotoFrame(this);
	}

    /**
	 * the frame index
	 */
	public int frame;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof GotoFrame))
        {
            GotoFrame gotoFrame = (GotoFrame) object;

            if (gotoFrame.frame == this.frame)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
