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

package flash.swf.types;

import flash.swf.Tag;
import flash.swf.TagHandler;
import java.util.List;

/**
 * @author Clement Wong
 */
public abstract class ShapeRecord
{
    public void visitDependents(TagHandler h)
    {
    }

    public void getReferenceList( List<Tag> list )
    {
    }

    public boolean equals( Object o )
    {
        return (o instanceof ShapeRecord);
    }

    public abstract void addToDelta(int xTwips, int yTwips);
}
