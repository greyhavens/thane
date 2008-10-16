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

package flash.fonts;

import flash.util.IntMapLRUCache;

/**
 * A <code>FontSet</code> is a collection of styles or "faces" for a given font.
 * Each <code>FontFace</code> is associated with a <code>java.awt.Font</code> instance
 * and contains a cache of converted glyphs, or character shape outlines.
 *
 * @author Peter Farland
 */
public class FontSet
{
    public FontSet(int maxFacesPerFont)
    {
        entries = new IntMapLRUCache(maxFacesPerFont, maxFacesPerFont)
        {

            public Object fetch(int key)
            {
                throw new UnsupportedOperationException();
            }
        };
    }

    public FontFace put(int style, FontFace entry)
    {
        return (FontFace)entries.put(style, entry);
    }

    public FontFace get(int style)
    {
        return (FontFace)entries.get(style);
    }

    private IntMapLRUCache entries;

}
