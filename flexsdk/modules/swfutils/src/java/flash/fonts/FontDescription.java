////////////////////////////////////////////////////////////////////////////////
//
// ADOBE SYSTEMS INCORPORATED
// Copyright 2003-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE: Adobe permits you to use, modify, and distribute this file
// in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.fonts;

/**
 * A FontDescription provides FontManagers a context that describes how to
 * locate a font for embedding, the font style characteristics and any options
 * that may control how it is to be embedded.  
 */
public class FontDescription
{
    /**
     * The name to use to register the font with the SWF.
     */
    public String alias;

    /**
     * The source of the font information, typically a URL pointing to a font
     * file.
     * 
     * The source may alternatively be just a String representing the font
     * family name of a font installed locally on the operating system.
     */
    public Object source;

    /**
     * The font style, represented as an int.
     * 
     * Plain is 0, Bold is 1, Italic is 2, and Bold+Italic is 3.
     */
    public int style;

    /**
     * The unicode characters to include in the DefineFont, or pass null to
     * include all available characters.
     */
    public String unicodeRanges;

    /**
     * Controls whether advanced anti-aliasing information should be included
     * (if it is available).
     */
    public boolean advancedAntiAliasing;

    /**
     * Controls whether the font should be embedded using compact font format
     * (if supported).
     */
    public boolean compactFontFormat;
}
