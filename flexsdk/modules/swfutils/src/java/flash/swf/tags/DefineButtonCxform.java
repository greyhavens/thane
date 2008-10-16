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
import flash.swf.types.CXForm;

/**
 * @author Clement Wong
 */
public class DefineButtonCxform extends Tag
{
    public DefineButtonCxform()
	{
		super(stagDefineButtonCxform);
	}

    public void visit(TagHandler h)
	{
		h.defineButtonCxform(this);
	}

	protected Tag getSimpleReference()
    {
        return button;
    }

    public DefineButton button;
	public CXForm colorTransform;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineButtonCxform))
        {
            DefineButtonCxform defineButtonCxform = (DefineButtonCxform) object;
            
            if ( equals(defineButtonCxform.button, this.button) &&
                 equals(defineButtonCxform.colorTransform, this.colorTransform))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
