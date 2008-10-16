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
import flash.swf.TagHandler;

/**
 * @author Clement Wong
 */
public class VideoFrame extends Tag
{
    public VideoFrame()
	{
		super(stagVideoFrame);
	}

    public void visit(TagHandler h)
	{
		h.videoFrame(this);
	}

	public Tag getSimpleReference()
    {
        return stream;
    }

    public DefineVideoStream stream;
	public int frameNum;
	public byte[] videoData;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof VideoFrame))
        {
            VideoFrame videoFrame = (VideoFrame) object;

            if ( equals(videoFrame.stream, this.stream) &&
                 (videoFrame.frameNum == this.frameNum) &&
                 Arrays.equals(videoFrame.videoData, this.videoData) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
