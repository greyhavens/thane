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
import flash.swf.types.ButtonCondAction;
import flash.swf.types.ButtonRecord;
import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * @author Clement Wong
 */
public class DefineButton extends DefineTag
{
    public DefineButton(int code)
    {
        super(code);
    }

    public void visit(TagHandler h)
	{
        if (code == stagDefineButton)
    		h.defineButton(this);
        else
            h.defineButton2(this);
	}

	public Iterator<Tag> getReferences()
    {
        return new Iterator<Tag>()
        {
            private int record = 0;

            public boolean hasNext()
            {
				// skip null entries
				while (record < buttonRecords.length && buttonRecords[record].characterRef == null)
					record++;
                return record < buttonRecords.length;
            }
            public Tag next()
            {
                if ( !hasNext() )
                    throw new NoSuchElementException();
                return buttonRecords[record++].characterRef;
            }
            public void remove()
            {
                throw new UnsupportedOperationException();
            }
        };
    }

	public ButtonRecord[] buttonRecords;
    public DefineButtonSound sounds;
    public DefineButtonCxform cxform;
    public DefineScalingGrid scalingGrid;

    /**
     * false = track as normal button
     * true = track as menu button
     */
    public boolean trackAsMenu;

    /**
     * actions to execute at particular button events.  For defineButton
     * this will only have one entry.  For defineButton2 it could have more
     * than one entry for different conditions.
     */
    public ButtonCondAction[] condActions;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineButton))
        {
            DefineButton defineButton = (DefineButton) object;

            if ( Arrays.equals(defineButton.buttonRecords, this.buttonRecords) &&
                 equals(defineButton.sounds, this.sounds) &&
                 equals(defineButton.cxform, this.cxform) &&
                 (defineButton.trackAsMenu == this.trackAsMenu) &&
                 Arrays.equals(defineButton.condActions, this.condActions) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
