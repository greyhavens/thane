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
import flash.swf.types.Matrix;
import flash.swf.types.Rect;
import flash.swf.types.TextRecord;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * @author Clement Wong
 */
public class DefineText extends DefineTag
{
    public DefineText(int code)
    {
        super(code);
        records=new LinkedList<TextRecord>();
    }

    public void visit(TagHandler h)
	{
        if (code == stagDefineText)
    		h.defineText(this);
        else
            h.defineText2(this);
	}

	public Iterator<Tag> getReferences()
    {
        LinkedList<Tag> refs = new LinkedList<Tag>();
        for (int i = 0; i < records.size(); ++i )
            records.get(i).getReferenceList( refs );

        return refs.iterator();
    }

    public Rect bounds;
    public Matrix matrix;
    public List<TextRecord> records;
    public CSMTextSettings csmTextSettings;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineText))
        {
            DefineText defineText = (DefineText) object;

            if ( equals(defineText.bounds, this.bounds) &&
                 equals(defineText.matrix, this.matrix) &&
                 equals(defineText.records, this.records))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int hashCode()
    {
      int hashCode = super.hashCode();

      if (bounds!=null)
      {
        hashCode += bounds.hashCode();
      }

      if (records!=null) {
        hashCode += records.size();
      }
      return hashCode;
    }
}
