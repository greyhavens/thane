////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2006 Adobe Systems Incorporated
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
 * @author Brian Deitte
 */
public class DefineFontAlignZones extends DefineTag
{
    public DefineFontAlignZones()
    {
        super(stagDefineFontAlignZones);
    }

    public void visit(TagHandler h)
	{
		   h.defineFontAlignZones(this);
	}

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineFontAlignZones))
        {
            DefineFontAlignZones alignZones = (DefineFontAlignZones)object;

            if (font.equals(alignZones.font) &&
                csmTableHint == alignZones.csmTableHint &&
                Arrays.equals(zoneTable, alignZones.zoneTable))
            {
                isEqual = true;
            }
        }
        return isEqual;
    }

    public DefineFont3 font;
    public int csmTableHint;
    public ZoneRecord[] zoneTable;
}
