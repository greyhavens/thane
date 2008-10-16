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
import flash.swf.TagHandler;

/**
 * @author Clement Wong
 */
public class SetBackgroundColor extends Tag
{
	public SetBackgroundColor()
	{
		super(stagSetBackgroundColor);
	}

    public SetBackgroundColor(int color)
    {
        this();
        this.color = color;
    }

    public void visit(TagHandler h)
	{
		h.setBackgroundColor(this);
	}

    /** color as int: 0x00RRGGBB */
    public int color;

    public int hashCode()
    {
        return super.hashCode() ^ color;
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof SetBackgroundColor))
        {
            SetBackgroundColor setBackgroundColor = (SetBackgroundColor) object;

            if (setBackgroundColor.color == this.color)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
