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
import java.util.Iterator;
import java.util.List;

/**
 * @author Clement Wong
 */
public class Shape
{
	public List<ShapeRecord> shapeRecords;

    public void visitDependents(TagHandler h)
    {
        Iterator<ShapeRecord> it = shapeRecords.iterator();
        while (it.hasNext())
        {
            ShapeRecord rec = it.next();
            rec.visitDependents(h);
        }
    }

    public void getReferenceList( List<Tag> refs )
    {
        Iterator<ShapeRecord> it = shapeRecords.iterator();

        while (it.hasNext())
        {
            ShapeRecord rec = it.next();
            rec.getReferenceList( refs );
        }
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof Shape)
        {
            Shape shape = (Shape) object;

            if ( ( (shape.shapeRecords == null) && (this.shapeRecords == null) ) ||
                 ( (shape.shapeRecords != null) && (this.shapeRecords != null) &&
                   ArrayLists.equals( shape.shapeRecords, this.shapeRecords ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
