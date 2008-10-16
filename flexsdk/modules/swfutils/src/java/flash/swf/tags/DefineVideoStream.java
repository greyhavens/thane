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

import flash.swf.TagHandler;

/**
 * @author Clement Wong
 */
public class DefineVideoStream extends DefineTag
{
	public DefineVideoStream()
	{
		super(stagDefineVideoStream);
	}

    public void visit(TagHandler h)
	{
		h.defineVideoStream(this);
	}

	public int numFrames;
	public int width;
	public int height;
	public int deblocking;
	public boolean smoothing;
	public int codecID;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineVideoStream))
        {
            DefineVideoStream defineVideoStream = (DefineVideoStream) object;

            if ( (defineVideoStream.numFrames == this.numFrames) &&
                 (defineVideoStream.width == this.width) &&
                 (defineVideoStream.height == this.height) &&
                 (defineVideoStream.deblocking == this.deblocking) &&
                 (defineVideoStream.smoothing == this.smoothing) &&
                 (defineVideoStream.codecID == this.codecID) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
