////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import java.util.Arrays;

/**
 * @author Roger Gonzalez
 */
public class DefineBinaryData extends DefineTag
{
    public DefineBinaryData()
    {
        super(stagDefineBinaryData);
    }
    public void visit(flash.swf.TagHandler h)
    {
        h.defineBinaryData(this);
    }
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineBinaryData))
        {
            DefineBinaryData defineBinaryData = (DefineBinaryData) object;

            if ( Arrays.equals(defineBinaryData.data, this.data) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int reserved;
    public byte[] data;

}
