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
public class RemoveObject extends Tag
{
	public RemoveObject(int code)
	{
		super(code);
	}

    public void visit(TagHandler h)
	{
        if (code == stagRemoveObject)
    		h.removeObject(this);
        else
            h.removeObject2(this);
	}

	public Tag getSimpleReference()
    {
        return ref;
    }

    public int depth;
    public DefineTag ref;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof RemoveObject))
        {
            RemoveObject removeObject = (RemoveObject) object;

            if ( (removeObject.depth == this.depth) &&
                 equals(removeObject.ref, this.ref) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
