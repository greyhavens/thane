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
import flash.swf.types.Rect;
import flash.swf.types.ShapeWithStyle;

import java.util.Iterator;
import java.util.LinkedList;

/**
 * @author Clement Wong
 */
public class DefineShape extends DefineTag
{
    public DefineShape(int code)
    {
        super(code);
    }

	public void visit(TagHandler h)
	{
        switch(code)
        {
        case stagDefineShape:
            h.defineShape(this);
            break;
        case stagDefineShape2:
            h.defineShape2(this);
            break;
        case stagDefineShape3:
            h.defineShape3(this);
            break;
        case stagDefineShape6:
            h.defineShape6(this);
            break;
        default:
            assert (false);
            break;
        }
	}

    public Iterator<Tag> getReferences()
    {
        LinkedList<Tag> refs = new LinkedList<Tag>();

        shapeWithStyle.getReferenceList( refs );

        return refs.iterator();
    }

	public Rect bounds;
	public ShapeWithStyle shapeWithStyle;
    public boolean usesNonScalingStrokes;
    public boolean usesScalingStrokes;
    public Rect edgeBounds;

    public DefineScalingGrid scalingGrid;
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineShape))
        {
            DefineShape defineShape = (DefineShape) object;

            if ( equals(defineShape.bounds, this.bounds) &&
                 equals(defineShape.shapeWithStyle, this.shapeWithStyle) &&
                 equals(defineShape.edgeBounds, this.edgeBounds) &&
                 (defineShape.usesNonScalingStrokes == this.usesNonScalingStrokes) &&
                  (defineShape.usesScalingStrokes == this.usesScalingStrokes))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int hashCode() {
      int hashCode = super.hashCode();
      hashCode += DefineTag.PRIME * bounds.hashCode();
      if (shapeWithStyle.shapeRecords !=null) {
        hashCode += DefineTag.PRIME * shapeWithStyle.shapeRecords.size();
      }
      if (shapeWithStyle.linestyles !=null) {
        hashCode += DefineTag.PRIME * shapeWithStyle.linestyles.size();
      }
      return hashCode;
    }

}
