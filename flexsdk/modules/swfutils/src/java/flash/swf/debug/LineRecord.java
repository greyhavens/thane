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

package flash.swf.debug;

import flash.swf.Action;
import flash.swf.ActionHandler;
import flash.swf.types.ActionList;

public class LineRecord extends Action
{
	public LineRecord(int lineno, DebugModule module)
	{
		super(ActionList.sactionLineRecord);
		this.lineno = lineno;
		this.module = module;
	}

    public int lineno;
    public DebugModule module;

	public void visit(ActionHandler h)
	{
		h.lineRecord(this);
	}

    public String toString()
    {
        return module.name + ":" + lineno;
    }

    public boolean equals(Object object)
    {
        if (object instanceof LineRecord)
        {
            LineRecord other = (LineRecord) object;
            return super.equals(other) &&
                    other.lineno == this.lineno &&
                    equals(other.module, this.module);
        }
        else
        {
            return false;
        }
    }
}
