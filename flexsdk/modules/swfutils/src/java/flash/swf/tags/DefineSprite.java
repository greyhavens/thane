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
import flash.swf.Header;
import flash.swf.types.TagList;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * @author Clement Wong
 */
public class DefineSprite extends DefineTag
{
    public DefineSprite()
	{
		super(stagDefineSprite);
        this.tagList = new TagList();
	}

    public DefineSprite(String name)
    {
        this();
        this.name = name;
    }

    public DefineSprite(DefineSprite source) // semi-shallow copy
    {
        this();
        this.name = source.name;
        this.tagList.tags.addAll( source.tagList.tags );
        this.initAction = source.initAction;
        this.framecount = source.framecount;
        this.header = source.header;
        if (source.scalingGrid != null)
        {
            scalingGrid = new DefineScalingGrid();
            scalingGrid.scalingTarget = this;
            scalingGrid.rect.xMin = source.scalingGrid.rect.xMin;
            scalingGrid.rect.xMax = source.scalingGrid.rect.xMax;
            scalingGrid.rect.yMin = source.scalingGrid.rect.yMin;
            scalingGrid.rect.yMax = source.scalingGrid.rect.yMax;
        }
    }

    public void visit(TagHandler h)
	{
		h.defineSprite(this);
	}

	public Iterator<Tag> getReferences()
    {
		ArrayList<Tag> list = new ArrayList<Tag>();
		for (Iterator i = tagList.tags.iterator(); i.hasNext();)
		{
			Tag tag = (Tag) i.next();
			for (Iterator<Tag> j = tag.getReferences(); j.hasNext();)
			{
				list.add(j.next());
			}
		}
		return list.iterator();
    }

	public int framecount;
	public TagList tagList;
    public DoInitAction initAction;
    public DefineScalingGrid scalingGrid;

    // the header of the SWF this sprite originally came from.  Tells us its framerate and SWF version.
    public Header header;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineSprite))
        {
            DefineSprite defineSprite = (DefineSprite) object;

            if ( (defineSprite.framecount == this.framecount) &&
                    equals(defineSprite.tagList, this.tagList) &&
                    equals(defineSprite.scalingGrid, this.scalingGrid) &&
                    equals(defineSprite.initAction, this.initAction))
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
            hashCode += name.hashCode();
        }

        hashCode += DefineTag.PRIME * framecount;
        if (tagList.tags!=null) {
          hashCode += DefineTag.PRIME * tagList.tags.size();
        }
        if (initAction !=null) {
          if (initAction.actionList!=null) {
            hashCode += DefineTag.PRIME * initAction.actionList.size();
          }
        }
        return hashCode;
    }

    public String toString()
    {
        StringBuilder stringBuffer = new StringBuilder();

        stringBuffer.append("DefineSprite: name = " + name +
                            ", framecount = " + framecount +
                            ", tagList = " + tagList +
                            ", initAction = " + initAction);

        return stringBuffer.toString();
    }

/*
    private void fitRect(Rect inner, Rect outer)
    {
        if (outer.xMin == 0 && outer.xMax == 0 && outer.yMin==0 && outer.yMax==0)
        {
            outer.xMin = inner.xMin;
            outer.xMax = inner.xMax;
            outer.yMin = inner.yMin;
            outer.yMax = inner.yMax;
        }
        else
        {
            outer.xMin = inner.xMin < outer.xMin ? inner.xMin : outer.xMin;
            outer.yMin = inner.yMin < outer.yMin ? inner.yMin : outer.yMin;
            outer.xMax = inner.xMax > outer.xMax ? inner.xMax : outer.xMax;
            outer.yMax = inner.yMax > outer.yMax ? inner.yMax : outer.yMax;
        }
    }

    public Rect getBounds()
    {
        Iterator it = timeline.tags.iterator();
        Rect bounds = new Rect();
        loop: while (it.hasNext())
        {
            Tag t = (Tag) it.next();
            switch (t.code)
            {
            case stagShowFrame:
                // stop at end of first frame
                break loop;
            case stagPlaceObject:
            case stagPlaceObject2:
                PlaceObject po = (PlaceObject) t;
                switch (po.ref.code)
                {
                case stagDefineEditText:
                    // how to calculate bounds?
                    break;
                case stagDefineSprite:
                    DefineSprite defineSprite = (DefineSprite) po.ref;
                    Rect spriteBounds = defineSprite.getBounds();
                    Rect newBounds = po.hasMatrix() ? po.matrix.xformRect(spriteBounds) : spriteBounds;
                    fitRect(newBounds, bounds);
                    break;
                case stagDefineShape:
                case stagDefineShape2:
                case stagDefineShape3:
                    DefineShape defineShape = (DefineShape) po.ref;
                    fitRect(defineShape.bounds, bounds);
                    break;
                }
                break;
            }
        }
        return bounds;
    } */
}
