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
import flash.swf.tags.DefineFont;

import java.util.Arrays;
import java.util.List;

/**
 * @author Clement Wong
 */
public class TextRecord
{
    private static final int HAS_FONT = 8;
    private static final int HAS_COLOR = 4;
    private static final int HAS_X = 1;
    private static final int HAS_Y = 2;
    private static final int HAS_HEIGHT = 8; // yep, same as HAS_FONT.  see player/core/stags.h
    public int flags = 128;

    /** color as integer 0x00RRGGBB or 0xAARRGGBB */
    public int color;

    public int xOffset;
    public int yOffset;
    public int height;
    public DefineFont font;
    public GlyphEntry[] entries;

    public void getReferenceList( List<Tag> refs )
    {
        if (hasFont() && font != null)
            refs.add( font );
    }

    public boolean hasFont()
    {
        return (flags & HAS_FONT) != 0;
    }

    public boolean hasColor()
    {
        return (flags & HAS_COLOR) != 0;
    }

    public boolean hasX()
    {
        return (flags & HAS_X) != 0;
    }

    public boolean hasY()
    {
        return (flags & HAS_Y) != 0;
    }

    public boolean hasHeight()
    {
        return (flags & HAS_HEIGHT) != 0;
    }

    public void setFont(DefineFont font)
    {
        this.font = font;
        flags |= HAS_FONT;
    }

    public void setHeight(int i)
    {
        this.height = i;
        flags |= HAS_HEIGHT;
    }

    public void setColor(int color)
    {
        flags |= HAS_COLOR;
        this.color = color;
    }

    public void setX(int xOffset)
    {
        this.xOffset = xOffset;
        flags |= HAS_X;
    }

    public void setY(int yOffset)
    {
        this.yOffset = yOffset;
        flags |= HAS_Y;
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof TextRecord)
        {
            TextRecord textRecord = (TextRecord) object;

            if ( (textRecord.flags == this.flags) &&
                 (textRecord.color == this.color) &&
                 (textRecord.xOffset == this.xOffset) &&
                 (textRecord.yOffset == this.yOffset) &&
                 (textRecord.height == this.height) &&
                 (textRecord.font == this.font) &&
                 ( Arrays.equals(textRecord.entries, this.entries) ) )

            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
