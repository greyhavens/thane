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
public class GotoFrame2 extends Action
{
	public GotoFrame2()
	{
		super(ActionConstants.sactionGotoFrame2);
	}

    public void visit(ActionHandler h)
	{
		h.gotoFrame2(this);
	}

    /**
	 * if the play flag is set, the action goes to the specified frame and
	 * begins playing the enclosed movie clip.  Otherwise, the action goes
	 * to the specified frame and stops.
	 */
	public int playFlag;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof GotoFrame2))
        {
            GotoFrame2 gotoFrame2 = (GotoFrame2) object;

            if (gotoFrame2.playFlag == this.playFlag)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
