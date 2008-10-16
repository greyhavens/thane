////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.fonts;

import flash.swf.types.GlyphEntry;
import flash.util.IntMapLRUCache;

/**
 * Provides a simple LRU cache layer to a FontFace.
 *
 * @author Peter Farland
 */
public abstract class CachedFontFace extends FontFace
{
    protected int style;
    protected FSType fsType;
    protected String copyright;
    protected String trademark;
    public boolean useTwips;
    public static final short GLYPH_CACHE_PURGE = 10;
    public final GlyphCache glyphCache;

    protected CachedFontFace(int maxCachedGlyphs)
    {
        glyphCache = new GlyphCache(this, maxCachedGlyphs / 5, maxCachedGlyphs, GLYPH_CACHE_PURGE);
    }

    protected CachedFontFace(int maxCachedGlyphs, int style, FSType fsType, String copyright, String trademark, boolean useTwips)
    {
        this(maxCachedGlyphs);
        this.style = style;
	    this.fsType = fsType;
	    this.copyright = copyright;
	    this.trademark = trademark;
	    this.useTwips = useTwips;
    }

    public boolean isBold()
    {
        return isBold(style);
    }

    public boolean isItalic()
    {
        return isItalic(style);
    }

	public FSType getFSType()
	{
		return fsType;
	}

	public void setFSType(FSType t)
	{
		this.fsType = t;
	}

	public String getCopyright()
	{
		return copyright;
	}

	public void setCopyright(String c)
	{
		this.copyright = c;
	}

	public String getTrademark()
	{
		return trademark;
	}

	public void setTrademark(String t)
	{
		this.trademark = t;
	}

    /**
     * Checks if a style is BOLD (1) or BOLD-ITALIC (3)
     *
     * @param style
     * @return boolean if bold style
     */
    public static boolean isBold(int style)
    {
        return style == BOLD || style == BOLD + ITALIC;
    }

    /**
     * Checks if a style is ITALIC (2) or BOLD-ITALIC (3)
     *
     * @param style
     * @return boolean if italic style
     */
    public static boolean isItalic(int style)
    {
        return style == ITALIC || style == ITALIC + BOLD;
    }

    public static int guessStyleFromSubFamilyName(String subFamilyName)
    {
        int style = PLAIN;

        if (subFamilyName != null)
        {
            subFamilyName = subFamilyName.toLowerCase();

            if (subFamilyName.indexOf("regular") != -1)
            {
                style = PLAIN;
            }

            if (subFamilyName.indexOf("bold") != -1)
            {
                style += BOLD;
            }

            if (subFamilyName.indexOf("italic") != -1 ||
                    subFamilyName.indexOf("oblique") != -1)
            {
                style += ITALIC;
            }
        }

        return style;
    }

    protected abstract GlyphEntry createGlyphEntry(char c);

    protected abstract GlyphEntry createGlyphEntry(char c, char referenceChar);


    static class GlyphCache extends IntMapLRUCache
    {
        private CachedFontFace fontFace;

        GlyphCache(CachedFontFace face, int initialSize, int maxSize, int purgeSize)
        {
            super(initialSize, maxSize, purgeSize);
            fontFace = face;
        }

        public Object fetch(int key)
        {
            char c = (char)key;

            if (fontFace.canDisplay(c))
            {
                return fontFace.createGlyphEntry(c, c);
            }
            else
            {
                return null;
            }
        }
    }
}
