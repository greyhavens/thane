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

import flash.swf.types.LineStyle;
import flash.swf.SwfConstants;
import flash.swf.SwfUtils;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Paint;
import java.awt.Stroke;

/**
 * @author Peter Farland
 */
public final class LineStyleBuilder
{
    private LineStyleBuilder()
    {
    }

    public static LineStyle build(Paint paint, Stroke thickness)
    {
        LineStyle ls = new LineStyle();

        if (paint != null && paint instanceof Color)
            ls.color = SwfUtils.colorToInt((Color)paint);

        if (thickness != null && thickness instanceof BasicStroke)
            ls.width = (int)(Math.rint(((BasicStroke)thickness).getLineWidth() * SwfConstants.TWIPS_PER_PIXEL));

        return ls;
    }
}
