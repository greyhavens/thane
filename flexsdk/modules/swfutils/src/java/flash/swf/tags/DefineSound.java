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
public class DefineSound extends DefineTag
{
	public DefineSound()
	{
		super(stagDefineSound);
	}

    public void visit(TagHandler h)
	{
		h.defineSound(this);
	}

	public int format;
	public int rate;
	public int size;
	public int type;
	public long sampleCount; // U32
	public byte[] data;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineSound))
        {
            DefineSound defineSound = (DefineSound) object;

            if ( (defineSound.format == this.format) &&
                 (defineSound.rate == this.rate) &&
                 (defineSound.size == this.size) &&
                 (defineSound.type == this.type) &&
                 (defineSound.sampleCount == this.sampleCount) &&
                 Arrays.equals(defineSound.data, this.data) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
