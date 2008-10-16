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

package flash.swf.types;

import flash.swf.TagHandler;
import flash.swf.tags.DefineTag;

/**
 * this is an import record, which is serialized as a member of an ImportAssets
 * tag.  We subclass DefineTag because definitions are the things that get
 * imported; any tag that refers to a definition can also refer to an import
 * of another definition.
 *
 * @author Edwin Smith
 */
public class ImportRecord extends DefineTag
{
    public ImportRecord()
    {
        super(stagImportAssets);
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof ImportRecord))
        {
            ImportRecord importRecord = (ImportRecord) object;

            if ( equals(importRecord.name, this.name))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }    

    public void visit(TagHandler h)
    {
        // this can't be visited, but you can visit the ImportAssets that owns this record.
       assert (false);
    }
}
