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
public class GotoLabel extends Action
{
	public GotoLabel()
	{
		super(ActionConstants.sactionGotoLabel);
	}

    public void visit(ActionHandler h)
	{
		h.gotoLabel(this);
	}

    /**
	 * Frame label, as attached to a frame using the FrameLabel tag.
	 */
	public String label;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof GotoLabel))
        {
            GotoLabel gotoLabel = (GotoLabel) object;

            if ( equals(gotoLabel.label, this.label))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
