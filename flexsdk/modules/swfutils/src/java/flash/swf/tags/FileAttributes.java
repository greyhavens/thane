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
 * FileAttributes defines whole-SWF attributes. It is a place to put information
 * that belongs in the SWF header, but for which the SWF header has no space.
 * FileAttributes must be the first tag in a SWF file, or it will be ignored.
 *
 * It is our hope that FileAttributes will be the only tag ever to have this
 * requirement of being located at a specific place in the file.
 * (Otherwise, a complicated set of ordering rules could ensue.)
 *
 * Any information that applies to the whole SWF should hopefully be incorporated
 * into the FileAttributes tag.
 *
 * @author Peter Farland
 */
public class FileAttributes extends Tag
{
    public boolean hasMetadata;
    public boolean actionScript3;
    public boolean suppressCrossDomainCaching;
    public boolean swfRelativeUrls;
    public boolean useNetwork;

    public FileAttributes()
    {
        super(stagFileAttributes);
    }

    public void visit(TagHandler handler)
    {
        handler.fileAttributes(this);
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof FileAttributes))
        {
            FileAttributes tag = (FileAttributes)object;

            if ((tag.hasMetadata == this.hasMetadata) &&
                (tag.actionScript3 == this.actionScript3) &&
                (tag.suppressCrossDomainCaching == this.suppressCrossDomainCaching) &&
                (tag.swfRelativeUrls == this.swfRelativeUrls) &&
                (tag.useNetwork == this.useNetwork))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

}
