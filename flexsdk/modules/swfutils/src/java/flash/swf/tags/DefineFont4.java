////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2008 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import java.util.Arrays;

import flash.swf.TagHandler;

/**
 * @author Peter Farland
 */
public class DefineFont4 extends DefineFont
{
    /**
     * Constructor.
     */
    public DefineFont4()
    {
        super(stagDefineFont4);
    }

    //--------------------------------------------------------------------------
    //
    // Fields and Bean Properties
    //
    //--------------------------------------------------------------------------

    public boolean hasFontData;
    public boolean smallText;
    public boolean italic;
    public boolean bold;
    public int langCode;
    public String fontName;
    public byte[] data;

    /**
     * The name of the font. This name is significant for embedded fonts at
     * runtime as it determines how one refers to the font in CSS. In SWF 6 and
     * later, font names are encoded using UTF-8.
     */
    public String getFontName()
    {
        return fontName;
    }

    /**
     * Reports whether the font face is bold.
     */
    public boolean isBold()
    {
        return bold;
    }

    /**
     * Reports whether the font face is italic.
     */
    public boolean isItalic()
    {
        return italic;
    }

    //--------------------------------------------------------------------------
    //
    // Visitor Methods
    //
    //--------------------------------------------------------------------------

    public void visit(TagHandler handler)
    {
        if (code == stagDefineFont4)
            handler.defineFont4(this);
    }

    //--------------------------------------------------------------------------
    //
    // Utility Methods
    //
    //--------------------------------------------------------------------------

    /**
     * Tests whether this DefineFont4 tag is equivalent to another DefineFont4
     * tag instance.
     * 
     * @param object Another DefineFont4 instance to test for equality.
     * @return true if the given instance is considered equal to this instance
     */
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof DefineFont4 && super.equals(object))
        {
            DefineFont4 defineFont = (DefineFont4)object;

            if ((defineFont.hasFontData == this.hasFontData) &&
                    (defineFont.italic == this.italic) &&
                    (defineFont.bold == this.bold) &&
                    (defineFont.langCode == this.langCode) &&
                    (defineFont.smallText == this.smallText) &&
                    equals(defineFont.fontName, this.fontName) &&
                    Arrays.equals(defineFont.data, this.data))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
