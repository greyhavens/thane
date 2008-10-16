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
import flash.swf.types.SoundInfo;

/**
 * @author Clement Wong
 */
public class StartSound extends Tag
{
    public StartSound()
	{
		super(stagStartSound);
	}

    public void visit(TagHandler h)
	{
		h.startSound(this);
	}

	public Tag getSimpleReference()
    {
        return sound;
    }

    /**
	 * ID of sound character to play
	 */
    public DefineSound sound;

	/**
	 * sound style information
	 */
	public SoundInfo soundInfo;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof StartSound))
        {
            StartSound startSound = (StartSound) object;

            if ( equals(startSound.sound, this.sound) &&
                 equals(startSound.soundInfo, this.soundInfo))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
