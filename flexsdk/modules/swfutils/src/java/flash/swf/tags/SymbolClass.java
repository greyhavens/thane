////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.tags;

import flash.swf.TagHandler;
import flash.swf.Tag;

import java.util.*;

/**
 * @author Clement Wong
 */
public class SymbolClass extends Tag
{
	public SymbolClass()
	{
		super(stagSymbolClass);
	}

    public void visit(TagHandler h)
	{
		h.symbolClass(this);
	}

	public Iterator<Tag> getReferences()
    {
		return class2tag.values().iterator();
    }

    public Map<String, Tag> class2tag = new HashMap<String, Tag>();
	public String topLevelClass;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof SymbolClass))
        {
            SymbolClass symbolClasses = (SymbolClass) object;

            if ( equals(symbolClasses.class2tag, this.class2tag) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
