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

public abstract class DefineTag extends Tag
{
    public DefineTag(int code)
    {
        super(code);
    }

    /** the export name of this symbol, or null if the symbol is not exported */
    public String name;
    private int id;
    public static final int PRIME=1000003;
    public int getID()
    {
        return id;
    }

    public void setID(int id)
    {
        this.id = id;
    }

    public String toString()
    {
        return name != null ? name : super.toString();
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineTag))
        {
            DefineTag defineTag = (DefineTag) object;

            if ( equals(defineTag.name, this.name) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int hashCode()
    {
        int hashCode = super.hashCode();

        if (name != null)
        {
            hashCode ^= name.hashCode()<<1;
        }

        return hashCode;
    }
}
