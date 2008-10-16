////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

/**
 * Stores the name and copyright information for a font.
 *
 * @author Brian Deitte
 */
public class DefineFontName extends DefineTag
{
    public DefineFontName()
    {
        super(stagDefineFontName);
    }

    public void visit(flash.swf.TagHandler h)
    {
        h.defineFontName(this);
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineFontName))
        {
            DefineFontName defineFontName = (DefineFontName) object;
            isEqual = (equals(font, defineFontName.font) &&
                       (equals(fontName, defineFontName.fontName)) &&
                       (equals(copyright, defineFontName.copyright)));
        }

        return isEqual;
    }

    public DefineFont font;
    public String fontName;
    public String copyright;
}
