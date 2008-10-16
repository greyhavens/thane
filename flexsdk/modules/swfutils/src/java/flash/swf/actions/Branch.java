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
public class Branch extends Action
{
	public Branch(int code)
	{
		super(code);
	}

    public void visit(ActionHandler h)
	{
        if (code == ActionConstants.sactionJump)
    		h.jump(this);
        else
            h.ifAction(this);
	}

    /**
	 * branch offset relative to the next instruction after the JUMP
	 */
    public Label target;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof Branch))
        {
            Branch branch = (Branch) object;
            
            if ( branch.target == this.target )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
