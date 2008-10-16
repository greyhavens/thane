////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import flash.swf.Tag;
import flash.swf.TagHandler;
import flash.swf.types.Rect;

/**
 * @author Roger Gonzalez
 */
public class DefineScalingGrid extends Tag
{
    public DefineScalingGrid()
    {
        super(stagDefineScalingGrid);

    }
    public DefineScalingGrid( DefineTag tag )
    {
        this();
        assert tag instanceof DefineSprite || tag instanceof DefineButton;

        if (tag instanceof DefineSprite)
        {
            ((DefineSprite)tag).scalingGrid = this;
        }

    }
    public void visit(TagHandler h)
    {
        h.defineScalingGrid( this );
    }

	protected Tag getSimpleReference()
    {
        return scalingTarget;
    }

    public boolean equals( Object other )
    {
        return ((other instanceof DefineScalingGrid)
                && ((DefineScalingGrid)other).scalingTarget == scalingTarget )
                && ((DefineScalingGrid)other).rect.equals( rect );
    }

    public DefineTag scalingTarget;
    public Rect rect = new Rect();

}
