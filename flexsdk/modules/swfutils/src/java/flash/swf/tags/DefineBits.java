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

import java.util.Arrays;

import flash.swf.Tag;

/**
 *
 * DefineBits
 * This tag defines a bitmap character with JPEG compression. It contains only the JPEG
 * compressed image data (from the Frame Header onward). A separate JPEGTables tag contains
 * the JPEG encoding data used to encode this image (the Tables/Misc segment). Note that only
 * one JPEGTables tag is allowed in a SWF file, and thus all bitmaps defined with DefineBits must
 * share common encoding tables. <p>
 *
 * The data in this tag begins with the JPEG SOI marker 0xFF, 0xD8 and ends with the EOI
 * marker 0xFF, 0xD9. <p>
 *
 * DefineBits2 - includes all jpeg data
 * DefineBits3 - includes all data plus a transparency map
 *
 * @since SWF1
 *
 * @author Clement Wong
 */
public class DefineBits extends DefineTag
{
    public DefineBits(int code)
    {
        super(code);
    }

    public void visit(flash.swf.TagHandler h)
	{
        if (code == stagDefineBitsJPEG2)
            h.defineBitsJPEG2(this);
        else
    		h.defineBits(this);
	}

	protected Tag getSimpleReference()
    {
        return jpegTables;
    }


	/** there is only one JPEG table in the entire movie */
    public GenericTag jpegTables;
	public byte[] data;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineBits))
        {
            DefineBits defineBits = (DefineBits) object;

            if ( Arrays.equals(defineBits.data, this.data) &&
                 equals(defineBits.jpegTables,  this.jpegTables) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
