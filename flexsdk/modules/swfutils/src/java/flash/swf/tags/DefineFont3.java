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

import flash.swf.TagHandler;

/**
 * The DefineFont3 tag extends the functionality of the DefineFont2 tag by
 * expressing the Shape coordinates in the glyph shape table at 20 times the
 * resolution. The EM square units are converted to twips to allow fractional
 * resolution to 1/20th of a unit. The DefineFont3 tag was introduced in SWF 8.
 * 
 * @author Clement Wong
 * @author Peter Farland
 */
public class DefineFont3 extends DefineFont2
{
    /**
     * Constructor.
     */
    public DefineFont3()
    {
        super(stagDefineFont3);
    }

    //--------------------------------------------------------------------------
    //
    // Fields and Bean Properties
    //
    //--------------------------------------------------------------------------

    public DefineFontAlignZones zones;

    //--------------------------------------------------------------------------
    //
    // Visitor Methods
    //
    //--------------------------------------------------------------------------

    /**
     * Invokes the defineFont visitor on the given TagHandler.
     * 
     * @param handler The SWF TagHandler.
     */
    public void visit(TagHandler handler)
    {
        if (code == stagDefineFont3)
            handler.defineFont3(this);
    }

    //--------------------------------------------------------------------------
    //
    // Utility Methods
    //
    //--------------------------------------------------------------------------

    /**
     * Tests whether this DefineFont3 tag is equivalent to another DefineFont3
     * tag instance.
     * 
     * @param object Another DefineFont3 instance to test for equality.
     * @return true if the given instance is considered equal to this instance
     */
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof DefineFont3 && super.equals(object))
        {
            DefineFont3 defineFont = (DefineFont3)object;

            // DefineFontAlignZones already checks if its font is equal, so we
            // don't check here to avoid circular equality checking...
            //if (equals(defineFont.zones, this.zones))

            isEqual = true;

        }

        return isEqual;
    }
}