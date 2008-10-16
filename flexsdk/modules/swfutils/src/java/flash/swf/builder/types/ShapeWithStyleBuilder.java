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

import flash.swf.types.ShapeWithStyle;
import flash.swf.types.FillStyle;
import flash.swf.types.LineStyle;

import java.util.ArrayList;
import java.awt.Shape;
import java.awt.Paint;
import java.awt.Stroke;

import flash.graphics.g2d.GraphicContext;

/**
 * @author Peter Farland
 */
public final class ShapeWithStyleBuilder
{
	public ShapeWithStyleBuilder(Shape shape, GraphicContext graphicContext, boolean outline, boolean fill)
	{
		Point origin = new Point(graphicContext.getPen().getX(), graphicContext.getPen().getY());
		Paint paint = graphicContext.getPaint();
		Stroke stroke = graphicContext.getStroke();

		FillStyle fs = null;
		LineStyle ls = null;

		if (fill && paint != null)
			fs = FillStyleBuilder.build(paint, shape.getBounds2D(), graphicContext.getTransform());

		if (outline && stroke != null)
			ls = LineStyleBuilder.build(paint, stroke);

		init(shape, origin, fs, ls, fill);
	}

	public ShapeWithStyleBuilder(Shape shape, Point origin, FillStyle fs, LineStyle ls, boolean fill)
	{
		init(shape, origin, fs, ls, fill);
	}

	private void init(Shape shape, Point origin, FillStyle fs, LineStyle ls, boolean fill)
	{
		builder = new ShapeBuilder(origin);
		linestyles = new ArrayList<LineStyle>(2);
		fillstyles = new ArrayList<FillStyle>(2);

		if (fill && fs != null)
		{
			builder.setUseFillStyle0(true);
			builder.setCurrentFillStyle0(addFillStyle(fs));
		}

		if (ls != null)
			builder.setCurrentLineStyle(addLineStyle(ls));

		builder.processShape(new PathIteratorWrapper(shape.getPathIterator(null)));
	}

	public ShapeWithStyle build()
	{
		ShapeWithStyle sws = new ShapeWithStyle();
		sws.shapeRecords = builder.build().shapeRecords;
		sws.fillstyles = fillstyles;
		sws.linestyles = linestyles;

		return sws;
	}

	public void join(Shape shape)
	{
		builder.processShape(new PathIteratorWrapper(shape.getPathIterator(null)));
	}

	/**
	 * Adds a new fill style to the <code>FillStyleArray</code>. If the given <code>FillStyle</code>
	 * is <code>null</code>, the style is ignored and an index of 0 is returned.
	 * @param fs the new <code>FillStyle</code>
	 * @return index pointing to this fill style in the <code>FillStyleArray</code>
	 */
	public int addFillStyle(FillStyle fs)
	{
		return fillstyles.add(fs) ? fillstyles.lastIndexOf(fs) + 1 : 0; //Index in a 1-based array, 0 = none
	}

	/**
	 * Adds a new fill style to the <code>LineStyleArray</code>
	 * @param ls the new <code>LineStyle</code>
	 * @return index pointing to this line style in the <code>LineStyleArray</code>
	 */
	public int addLineStyle(LineStyle ls)
	{
		return linestyles.add(ls) ? linestyles.lastIndexOf(ls) + 1 : 0; //Index in a 1-based array, 0 = none
	}

	private ShapeBuilder builder;
    private ArrayList<LineStyle> linestyles;
	private ArrayList<FillStyle> fillstyles;
}
