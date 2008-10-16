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

import flash.swf.Tag;
import flash.swf.TagHandler;

/**
 * @author Clement Wong
 */
public class SoundStreamHead extends Tag
{
    public static final int sndCompressNone = 0;
    public static final int sndCompressADPCM = 1;
    public static final int sndCompressMP3 = 2;
    public static final int sndCompressNoneI = 3;

	public SoundStreamHead(int code)
	{
		super(code);
	}

    public void visit(TagHandler h)
	{
        if (code == stagSoundStreamHead)
		    h.soundStreamHead(this);
        else
            h.soundStreamHead2(this);
	}

	public int playbackRate;
	public int playbackSize;
	public int playbackType;
	public int compression;
	public int streamRate;
	public int streamSize;
	public int streamType;
	public int streamSampleCount;
	public int latencySeek;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof SoundStreamHead))
        {
            SoundStreamHead soundStreamHead = (SoundStreamHead) object;

            if ((soundStreamHead.playbackRate == this.playbackRate) &&
                (soundStreamHead.playbackSize == this.playbackSize) &&
                (soundStreamHead.playbackType == this.playbackType) &&
                (soundStreamHead.compression == this.compression) &&
                (soundStreamHead.streamRate == this.streamRate) &&
                (soundStreamHead.streamSize == this.streamSize) &&
                (soundStreamHead.streamType == this.streamType) &&
                (soundStreamHead.streamSampleCount == this.streamSampleCount) &&
                (soundStreamHead.latencySeek == this.latencySeek))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
