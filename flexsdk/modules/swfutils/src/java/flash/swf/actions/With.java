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
public class With extends Action
{
	public With()
	{
		super(ActionConstants.sactionWith);
	}


	public void visit(ActionHandler h)
	{
		h.with(this);
	}

	public Label endWith;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof With))
        {
            With with = (With) object;

            if ( with.endWith == this.endWith)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
