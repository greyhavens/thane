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
public class DefineBitsJPEG3 extends DefineBits
{
	public DefineBitsJPEG3()
	{
		super(stagDefineBitsJPEG3);
	}

    public void visit(TagHandler h)
	{
		h.defineBitsJPEG3(this);
	}

	public long alphaDataOffset;
	public byte[] alphaData;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineBitsJPEG3))
        {
            DefineBitsJPEG3 defineBitsJPEG3 = (DefineBitsJPEG3) object;

            if ( (defineBitsJPEG3.alphaDataOffset == this.alphaDataOffset) &&
                 Arrays.equals(defineBitsJPEG3.alphaData, this.alphaData) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
