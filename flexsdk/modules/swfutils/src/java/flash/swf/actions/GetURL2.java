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
public class GetURL2 extends Action
{
	public GetURL2()
	{
		super(ActionConstants.sactionGetURL2);
	}

    public void visit(ActionHandler h)
	{
		h.getURL2(this);
	}

    /**
	 * 0 = None. Not a form request, don't encode variables.
	 * 1 = GET. variables encoded as a query string
	 * 2 = POST.  variables encoded as http request body
	 */
	public int method;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof GetURL2))
        {
            GetURL2 getURL2 = (GetURL2) object;

            if (getURL2.method == this.method)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
