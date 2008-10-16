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
public class WaitForFrame extends Action
{
	public WaitForFrame(int code)
	{
		super(code);
	}

    public void visit(ActionHandler h)
	{
		if (code == ActionConstants.sactionWaitForFrame)
			h.waitForFrame(this);
		else
			h.waitForFrame2(this);
	}

    /**
	 * Frame number to wait for (WaitForFrame only).  WaitForFrame2 takes
     * its frame argument from the stack.
	 */
	public int frame;

	/**
	 *  label marking the number of actions to skip if frame is not loaded
	 */
	public Label skipTarget;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof WaitForFrame))
        {
            WaitForFrame waitForFrame = (WaitForFrame) object;

            if ( (waitForFrame.frame == this.frame) && 
                 (waitForFrame.skipTarget == this.skipTarget) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
