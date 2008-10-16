////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import flash.swf.Tag;
import flash.swf.TagHandler;
import flash.swf.types.ImportRecord;

import java.util.List;

/**
 * @author Clement Wong
 */
public class ImportAssets extends Tag
{
	public ImportAssets(int code)
	{
		super(code);
	}

    public void visit(TagHandler h)
	{
		if (code == stagImportAssets)
			h.importAssets(this);
		else
			h.importAssets2(this);
	}

    public String url;
	public List<ImportRecord> importRecords;

    public boolean downloadNow;
    public byte[] SHA1;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof ImportAssets))
        {
            ImportAssets importAssets = (ImportAssets) object;

            if ( equals(importAssets.url, this.url) &&
            	 (importAssets.downloadNow == this.downloadNow) &&
            	 digestEquals(importAssets.SHA1, this.SHA1) &&
                 equals(importAssets.importRecords,  this.importRecords))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
    
    private boolean digestEquals(byte[] d1, byte[] d2)
    {
    	if (d1 == null && d2 == null)
    	{
    		return true;
    	}
    	else
    	{
    		for (int i = 0; i < 20; i++)
    		{
    			if (d1[i] != d2[i])
    			{
    				return false;
    			}
    		}
    		return true;
    	}
    }
}
