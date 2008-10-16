////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.types;

import flash.swf.tags.ZoneRecord;

/**
 * @author Clement Wong
 */
public class GlyphEntry implements Cloneable
{
    private GlyphEntry original;
    private int index;
    public int advance;

    //Utilities for DefineFont
    public char character;
    public Rect bounds;
    public ZoneRecord zoneRecord;
    public Shape shape;

    public Object clone()
    {
        Object clone = null;

        try
        {
            clone = super.clone();
            ((GlyphEntry) clone).original = this;
        }
        catch (CloneNotSupportedException cloneNotSupportedException)
        {
            // preilly: We should never get here, but just in case print a stack trace.
            cloneNotSupportedException.printStackTrace();
        }

        return clone;
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof GlyphEntry)
        {
            GlyphEntry glyphEntry = (GlyphEntry) object;

            if ( (glyphEntry.index == this.index) &&
                 (glyphEntry.advance == this.advance) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int getIndex()
    {
        int result;

        if (original != null)
        {
            result = original.getIndex();
        }
        else
        {
            result = index;
        }

        return result;
    }

    public void setIndex(int index)
    {
        this.index = index;
    }

    // Retained for coldfusion.document.CFFontManager implementation
    public void setShape(Shape s)
    {
        this.shape = s;
    }

    public Shape getShape()
    {
        return this.shape;
    }
}
