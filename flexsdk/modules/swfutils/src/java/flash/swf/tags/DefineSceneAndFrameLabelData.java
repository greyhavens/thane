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

import flash.swf.Tag;
import flash.swf.TagHandler;

/**
 * @author Roger Gonzalez
 */
public class DefineSceneAndFrameLabelData extends Tag
{
    public DefineSceneAndFrameLabelData()
    {
        super( stagDefineSceneAndFrameLabelData );
    }
    public void visit( TagHandler h )
    {
        h.defineSceneAndFrameLabelData( this );
    }
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineSceneAndFrameLabelData))
        {
            isEqual = equals(((DefineSceneAndFrameLabelData)object).data, this.data);
        }
        return isEqual;
    }
    public int hashCode()
    {
        return data.hashCode();
    }
    // todo: once we care about this tag, break out the fields
    // for now, just allow round-tripping
    public byte[] data;

}
