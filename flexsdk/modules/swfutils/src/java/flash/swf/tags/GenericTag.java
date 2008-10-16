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

/**
 * Tag that just contains a byte[] payload.  We can use this to hold any tag
 * in its packed format, and also to hold tags that don't need any unpacking.
 * @author Clement Wong
 */
public class GenericTag extends flash.swf.Tag
{
    public GenericTag(int code)
    {
        super(code);
    }

    public void visit(flash.swf.TagHandler h)
	{
        switch (code)
        {
        case stagJPEGTables:
            h.jpegTables(this);
            break;
        case stagProtect:
            h.protect(this);
            break;
        case stagSoundStreamBlock:
            h.soundStreamBlock(this);
            break;
        default:
            h.unknown(this);
            break;
        }
	}

    public byte[] data;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof GenericTag))
        {
            GenericTag genericTag = (GenericTag) object;

            if ( equals(genericTag.data, this.data) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
