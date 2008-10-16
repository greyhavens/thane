////////////////////////////////////////////////////////////////////////////////
//
// ADOBE SYSTEMS INCORPORATED
// Copyright 2003-2006 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE: Adobe permits you to use, modify, and distribute this file
// in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import java.util.Arrays;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import flash.swf.Tag;
import flash.swf.TagHandler;
import flash.swf.types.Shape;

/**
 * A DefineFont tag defines the shape outlines of each glyph used in a
 * particular font. Only the glyphs that are used by subsequent DefineText tags
 * should be included. DefineFont tags cannot be used for dynamic text. Dynamic
 * text requires the DefineFont2 tag. DefineFont was introduced in SWF version
 * 1.
 * 
 * @see DefineFontInfo
 * @author Clement Wong
 * @author Peter Farland
 */
public class DefineFont1 extends DefineFont
{
    /**
     * Constructor.
     */
    public DefineFont1()
    {
        super(stagDefineFont);
    }

    //--------------------------------------------------------------------------
    //
    // Fields and Bean Properties
    //
    //--------------------------------------------------------------------------

    /**
     * An Array of Shapes representing glyph outlines.
     */
    public Shape[] glyphShapeTable;

    /**
     * The DefineFontInfo associated with this DefineFont tag. DefineFontInfo
     * provides a mapping from this glyph font (i.e. DefineFont) to a device
     * font, as well as the font name and style information.
     */
    public DefineFontInfo fontInfo;

    /**
     * The name of the font. This name is significant for embedded fonts at
     * runtime as it determines how one refers to the font in CSS. In SWF 6 and
     * later, font names are encoded using UTF-8. In SWF 5 and earlier, font
     * names are encoded in a platform specific manner in the codepage of the
     * system they were authored.
     */
    public String getFontName()
    {
        if (fontInfo != null)
            return fontInfo.name;

        return null;
    }

    /**
     * Reports whether the font face is bold.
     */
    public boolean isBold()
    {
        if (fontInfo != null)
            return fontInfo.bold;

        return false;
    }

    /**
     * Reports whether the font face is italic.
     */
    public boolean isItalic()
    {
        if (fontInfo != null)
            return fontInfo.italic;

        return false;
    }

    //--------------------------------------------------------------------------
    //
    // Visitor Methods
    //
    //--------------------------------------------------------------------------

    /**
     * Find the immediate, first order dependencies.
     * 
     * @return Iterator of immediate references of this DefineFont.
     */
    public Iterator<Tag> getReferences()
    {
        List<Tag> refs = new LinkedList<Tag>();

        for (int i = 0; i < glyphShapeTable.length; i++)
            glyphShapeTable[i].getReferenceList(refs);

        return refs.iterator();
    }

    /**
     * Invokes the defineFont visitor on the given TagHandler.
     * 
     * @param handler The SWF TagHandler.
     */
    public void visit(TagHandler handler)
    {
        if (code == stagDefineFont)
            handler.defineFont(this);
    }

    //--------------------------------------------------------------------------
    //
    // Utility Methods
    //
    //--------------------------------------------------------------------------

    /**
     * Tests whether this DefineFont tag is equivalent to another DefineFont tag
     * instance.
     * 
     * @param object Another DefineFont (version 1) instance to test for
     *        equality.
     * @return true if the given instance is considered equal to this instance
     */
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof DefineFont1 && super.equals(object))
        {
            DefineFont1 defineFont = (DefineFont1)object;

            if (equals(defineFont.fontInfo, this.fontInfo) &&
                    Arrays.equals(defineFont.glyphShapeTable, this.glyphShapeTable))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
