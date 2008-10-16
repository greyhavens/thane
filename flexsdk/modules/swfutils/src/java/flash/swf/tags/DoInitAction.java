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
import flash.swf.types.ActionList;

/**
 * @author Clement Wong
 */
public class DoInitAction extends Tag
{
    public DoInitAction()
	{
		super(stagDoInitAction);
	}

    public DoInitAction(DefineSprite sprite)
    {
        this();
        this.sprite = sprite;
        sprite.initAction = this;
    }

    public void visit(TagHandler h)
	{
		h.doInitAction(this);
	}

	protected Tag getSimpleReference()
    {
        return sprite;
    }

    public DefineSprite sprite;
	public ActionList actionList;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DoInitAction))
        {
            DoInitAction doInitAction = (DoInitAction) object;

            assert (doInitAction.sprite.initAction == doInitAction);

            // [paul] Checking that the sprite fields are equal would
            // lead to an infinite loop, because DefineSprite contains
            // a reference to it's DoInitAction.  Also don't compare
            // the order fields, because they are never the same.
            if ( equals(doInitAction.actionList, this.actionList))
            {
                isEqual = true;
            }
        }

        assert (sprite.initAction == this);

        return isEqual;
    }
}
