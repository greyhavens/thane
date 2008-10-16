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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * ExportAssets makes portions of a SWF file available for import by other SWF files (see
 * ImportAssets). For example, ten Flash movies that are all part of the same website can share an
 * embedded custom font if one movie embeds the font and exports the font character. Each
 * exported character is identified by a string. Any type of character can be exported.
 * @author Clement Wong
 * @since SWF5
 */
public class ExportAssets extends Tag
{
	public ExportAssets()
	{
		super(stagExportAssets);
	}

    public void visit(TagHandler h)
	{
		h.exportAssets(this);
	}

	public Iterator<Tag> getReferences()
    {
        return exports.iterator();
    }

    /** list of DefineTags exported by this ExportTag */
    public List<Tag> exports = new ArrayList<Tag>();

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof ExportAssets))
        {
            ExportAssets exportAssets = (ExportAssets) object;

            if ( equals(exportAssets.exports, this.exports) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
