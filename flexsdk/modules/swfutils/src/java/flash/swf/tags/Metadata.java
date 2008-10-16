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

import flash.swf.Tag;
import flash.swf.TagHandler;

/**
 * @author Roger Gonzalez
 */
public class Metadata extends Tag
{
    public Metadata()
    {
        super(stagMetadata);
    }

    public void visit( TagHandler h)
    {
        h.metadata(this);
    }

    public boolean equals( Object o )
    {
        return ((o != null) && (o instanceof Metadata) && (((Metadata)o).xml).equals(xml));
    }

    public String xml;

}
