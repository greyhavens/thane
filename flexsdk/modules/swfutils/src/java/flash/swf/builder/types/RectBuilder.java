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

package flash.swf.builder.types;

import flash.swf.SwfConstants;
import flash.swf.types.Rect;

import java.awt.geom.Rectangle2D;

/**
 * @author Peter Farland
 */
public final class RectBuilder
{
    private RectBuilder()
    {
    }

    public static Rect build(Rectangle2D r)
    {
        Rect rect = new Rect();

        rect.xMin = (int)Math.rint(r.getMinX() * SwfConstants.TWIPS_PER_PIXEL);
        rect.yMin = (int)Math.rint(r.getMinY() * SwfConstants.TWIPS_PER_PIXEL);
        rect.xMax = (int)Math.rint(r.getMaxX() * SwfConstants.TWIPS_PER_PIXEL);
        rect.yMax = (int)Math.rint(r.getMaxY() * SwfConstants.TWIPS_PER_PIXEL);

        return rect;
    }
}
