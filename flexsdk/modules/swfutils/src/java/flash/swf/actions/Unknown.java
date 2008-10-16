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

import java.util.Arrays;

import flash.swf.Action;
import flash.swf.ActionHandler;

/**
 * @author Clement Wong
 */
public class Unknown extends Action
{
    public Unknown(int code)
    {
        super(code);
    }

    public void visit(ActionHandler handler)
	{
		handler.unknown(this);
	}

    public byte[] data;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof Unknown))
        {
            Unknown unknown = (Unknown) object;

            if ( Arrays.equals(unknown.data, this.data) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
