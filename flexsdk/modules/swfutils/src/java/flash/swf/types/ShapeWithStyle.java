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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import flash.swf.Tag;

/**
 * @author Clement Wong
 */
public class ShapeWithStyle extends Shape
{
    public ArrayList<FillStyle> fillstyles;
	public ArrayList<LineStyle> linestyles;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if ( super.equals(object) && (object instanceof ShapeWithStyle) )
        {
            ShapeWithStyle shapeWithStyle = (ShapeWithStyle) object;

            if ( ( ( (shapeWithStyle.fillstyles == null) && (this.fillstyles == null) ) ||
                   ( (shapeWithStyle.fillstyles != null) && (this.fillstyles != null) &&
                     ArrayLists.equals( shapeWithStyle.fillstyles,
                                    this.fillstyles ) ) ) &&
                 ( ( (shapeWithStyle.linestyles == null) && (this.linestyles == null) ) ||
                   ( (shapeWithStyle.linestyles != null) && (this.linestyles != null) &&
                     ArrayLists.equals( shapeWithStyle.linestyles,
                                    this.linestyles ) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public void getReferenceList( List<Tag> refs )
    {
        super.getReferenceList(refs);

        Iterator it = fillstyles.iterator();
        while (it.hasNext())
        {
            FillStyle style = (FillStyle) it.next();

            if (style.hasBitmapId() && style.bitmap != null)
            {
                refs.add( style.bitmap );
            }
        }
    }
}
