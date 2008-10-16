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
public class StrictMode extends Action
{
	public StrictMode(boolean mode)
	{
		super(ActionConstants.sactionStrictMode);
		this.mode = mode;
	}

	public void visit(ActionHandler h)
	{
		h.strictMode(this);
	}

    public boolean mode;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof StrictMode))
        {
            StrictMode strictMode = (StrictMode) object;

            if (strictMode.mode == this.mode)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
