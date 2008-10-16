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
import flash.swf.types.Shape;
import flash.swf.types.MorphFillStyle;
import flash.swf.types.MorphLineStyle;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * @author Clement Wong
 */
public class DefineMorphShape extends DefineTag
{
	public DefineMorphShape(int code)
	{
		super(code);
	}

    public void visit(TagHandler h)
	{
    	if (code == stagDefineMorphShape)
    		h.defineMorphShape(this);
    	else // if (code == stagDefineMorphShape2)
    		h.defineMorphShape2(this);
	}

	public Iterator<Tag> getReferences()
    {
        // This is yucky.
        List<Tag> refs = new LinkedList<Tag>();

        startEdges.getReferenceList( refs );
        endEdges.getReferenceList( refs );

        return refs.iterator();
    }

	public Rect startBounds;
	public Rect endBounds;
	public Rect startEdgeBounds;
	public Rect endEdgeBounds;
	public int reserved;
	public boolean usesNonScalingStrokes;
	public boolean usesScalingStrokes;
	public MorphFillStyle[] fillStyles;
	public MorphLineStyle[] lineStyles;
	public Shape startEdges;
	public Shape endEdges;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineMorphShape))
        {
            DefineMorphShape defineMorphShape = (DefineMorphShape) object;

            if ( defineMorphShape.code == this.code &&
            	 equals(defineMorphShape.startBounds, this.startBounds) &&
                 equals(defineMorphShape.endBounds, this.endBounds) &&
                 equals(defineMorphShape.fillStyles, this.fillStyles) &&
                 equals(defineMorphShape.lineStyles,  this.lineStyles) &&
                 equals(defineMorphShape.startEdges, this.startEdges) &&
                 equals(defineMorphShape.endEdges, this.endEdges) )
            {
                isEqual = true;
            	if (this.code == stagDefineMorphShape2)
            	{
            		isEqual = equals(defineMorphShape.startEdgeBounds, this.startEdgeBounds) &&
            				  equals(defineMorphShape.endEdgeBounds, this.endEdgeBounds) &&
            				  defineMorphShape.usesNonScalingStrokes == this.usesNonScalingStrokes &&
            				  defineMorphShape.usesScalingStrokes == this.usesScalingStrokes;
            	}
            }
        }

        return isEqual;
    }
}
