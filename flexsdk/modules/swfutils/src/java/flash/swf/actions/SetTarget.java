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
public class SetTarget extends Action
{
	public SetTarget()
	{
		super(ActionConstants.sactionSetTarget);
	}

    public void visit(ActionHandler h)
	{
		h.setTarget(this);
	}

    /**
	 * name of target object for subsequent actions.  Empty string ("") means
	 * the current movie.
	 */
	public String targetName;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof SetTarget))
        {
            SetTarget setTarget = (SetTarget) object;

            if ( equals(setTarget.targetName, this.targetName))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
