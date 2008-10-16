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

import java.util.Iterator;
import java.util.ArrayList;

/**
 * @author Clement Wong
 */
public class DefineButtonSound extends Tag
{
    public DefineButtonSound()
	{
		super(stagDefineButtonSound);
	}

    public void visit(TagHandler h)
	{
		h.defineButtonSound(this);
	}

	public Iterator<Tag> getReferences()
    {
        ArrayList<Tag> list = new ArrayList<Tag>(5);
        list.add(button);
        if (sound0 != null)
            list.add(sound0);
        if (sound1 != null)
            list.add(sound1);
        if (sound2 != null)
            list.add(sound2);
        if (sound3 != null)
            list.add(sound3);
        return list.iterator();
    }

	public DefineTag sound0;
	public SoundInfo info0;
	public DefineTag sound1;
	public SoundInfo info1;
	public DefineTag sound2;
	public SoundInfo info2;
	public DefineTag sound3;
	public SoundInfo info3;

    public DefineButton button;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineButtonSound))
        {
            DefineButtonSound defineButtonSound = (DefineButtonSound) object;

            // [ed] don't compare button because that would be infinite recursion
            if ( equals(defineButtonSound.sound0, this.sound0) &&
                 equals(defineButtonSound.info0, this.info0) &&
                 equals(defineButtonSound.sound1, this.sound1) &&
                 equals(defineButtonSound.info1, this.info1) &&
                 equals(defineButtonSound.sound2, this.sound2) &&
                 equals(defineButtonSound.info2, this.info2) &&
                 equals(defineButtonSound.sound3, this.sound3) &&
                 equals(defineButtonSound.info3, this.info3))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
