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

public class Try extends Action
{
    public Try()
    {
        super(ActionConstants.sactionTry);
    }

    public void visit(ActionHandler h)
    {
        h.tryAction(this);
    }

    public int flags;

	/** name of variable holding the error object in catch body */
    public String catchName;

	/** register that holds the catch variable */
	public int catchReg;

	/** marks end of body and start of catch handler */
	public Label endTry;
	/** marks end of catch handler and start of finally handler */
	public Label endCatch;
	/** marks end of finally handler */
	public Label endFinally;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof Try))
        {
            Try tryAction = (Try) object;

            if ( (tryAction.flags == this.flags) &&
                 equals(tryAction.catchName, this.catchName) &&
                 (tryAction.catchReg == this.catchReg) &&
                 tryAction.endTry == this.endTry &&
                 tryAction.endCatch == this.endCatch &&
                 tryAction.endFinally == this.endFinally )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

	public boolean hasCatch()
	{
		return (flags & ActionConstants.kTryHasCatchFlag) != 0;
	}

	public boolean hasFinally()
	{
		return (flags & ActionConstants.kTryHasFinallyFlag) != 0;
	}

	public boolean hasRegister()
	{
		return (flags & ActionConstants.kTryCatchRegisterFlag) != 0;
	}
}
