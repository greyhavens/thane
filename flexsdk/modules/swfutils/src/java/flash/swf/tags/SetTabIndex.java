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

import flash.swf.Tag;
import flash.swf.TagValues;
import flash.swf.TagHandler;

/**
 * @author Edwin Smith
 */
public class SetTabIndex extends Tag
{
    public SetTabIndex(int depth, int index)
    {
        super(TagValues.stagSetTabIndex);
        this.depth = depth;
        this.index = index;
    }

    public void visit(TagHandler tagHandler)
	{
        tagHandler.setTabIndex(this);
	}

    final public int depth;
    final public int index;

    public boolean equals(Object object)
    {
        if (super.equals(object) && object instanceof SetTabIndex)
        {
            SetTabIndex other = (SetTabIndex) object;
            return other.depth == this.depth && other.index == this.index;
        }
        else
        {
            return false;
        }
    }
}
