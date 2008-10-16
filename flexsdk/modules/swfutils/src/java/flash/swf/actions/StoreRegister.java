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
public class StoreRegister extends Action
{
	public StoreRegister(int register)
	{
		super(ActionConstants.sactionStoreRegister);
		this.register = register;
	}

    public void visit(ActionHandler h)
	{
		h.storeRegister(this);
	}

    public int register;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof StoreRegister))
        {
            StoreRegister storeRegister = (StoreRegister) object;

            if (storeRegister.register == this.register)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
