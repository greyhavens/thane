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

package flash.swf.tags;

/**
 * @author Clement Wong
 */
public class EnableDebugger extends flash.swf.Tag
{
	public EnableDebugger(int code)
	{
		super(code);
	}

    public EnableDebugger(String password)
    {
        super(stagEnableDebugger2);
        this.password = password;
    }

    public void visit(flash.swf.TagHandler h)
	{
        if (code == stagEnableDebugger)
    		h.enableDebugger(this);
        else
            h.enableDebugger2(this);
	}

    public String password;
	public int    reserved;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof EnableDebugger))
        {
            EnableDebugger enableDebugger = (EnableDebugger) object;

            if ( equals(enableDebugger.password, this.password) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
