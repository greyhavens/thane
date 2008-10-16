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

import flash.swf.TagHandler;

/**
 * @author Clement Wong
 */
public class DefineBitsLossless extends DefineBits
{
    public DefineBitsLossless(int code)
	{
		super(code);
	}

    public void visit(TagHandler h)
	{
        if (code == stagDefineBitsLossless)
    		h.defineBitsLossless(this);
        else
            h.defineBitsLossless2(this);
	}

	public int format;
	public int width;
	public int height;

    /**
     * DefineBitsLossLess:  array of 0x00RRGGBB
     * DefineBitsLossLess2: array of 0xAARRGGBB
     */
    public int[] colorData;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineBitsLossless))
        {
            DefineBitsLossless defineBitsLossless = (DefineBitsLossless) object;

            if ( (defineBitsLossless.format == this.format) &&
                 (defineBitsLossless.width == this.width) &&
                 (defineBitsLossless.height == this.height) &&
                 Arrays.equals(defineBitsLossless.colorData, this.colorData) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
