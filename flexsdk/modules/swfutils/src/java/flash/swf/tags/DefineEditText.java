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
import flash.swf.types.Rect;

import java.util.Iterator;
import java.util.LinkedList;

/**
 * @author Clement Wong
 */
public class DefineEditText extends DefineTag
{
    public DefineEditText()
	{
		super(stagDefineEditText);
	}

    public void visit(TagHandler h)
	{
		h.defineEditText(this);
	}

    public Iterator<Tag> getReferences()
    {
        LinkedList<Tag> refs = new LinkedList<Tag>();
        if (font != null)
            refs.add(font);

        return refs.iterator();
    }

	public Rect bounds;
	public boolean hasText;
	public boolean wordWrap;
	public boolean multiline;
	public boolean password;
	public boolean readOnly;
	public boolean hasTextColor;
	public boolean hasMaxLength;
	public boolean hasFont;
	public boolean autoSize;
	public boolean hasLayout;
	public boolean noSelect;
	public boolean border;
	public boolean wasStatic;
	public boolean html;
	public boolean useOutlines;

    public DefineFont font;
	public int height;
    /** color as int: 0xAARRGGBB */
	public int color;
	public int maxLength;
	public int align;
	public int leftMargin;
	public int rightMargin;
	public int ident;
	public int leading;
	public String varName;
	public String initialText;
    public CSMTextSettings csmTextSettings;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DefineEditText))
        {
            DefineEditText defineEditText = (DefineEditText) object;

            if ( equals(defineEditText.bounds, this.bounds) &&
                 (defineEditText.hasText == this.hasText) &&
                 (defineEditText.wordWrap == this.wordWrap) &&
                 (defineEditText.multiline == this.multiline) &&
                 (defineEditText.password == this.password) &&
                 (defineEditText.readOnly == this.readOnly) &&
                 (defineEditText.hasTextColor == this.hasTextColor) &&
                 (defineEditText.hasMaxLength == this.hasMaxLength) &&
                 (defineEditText.hasFont == this.hasFont) &&
                 (defineEditText.autoSize == this.autoSize) &&
                 (defineEditText.hasLayout == this.hasLayout) &&
                 (defineEditText.noSelect == this.noSelect) &&
                 (defineEditText.border == this.border) &&
                 (defineEditText.wasStatic == this.wasStatic) &&
                 (defineEditText.html == this.html) &&
                 (defineEditText.useOutlines == this.useOutlines) &&
                 equals(defineEditText.font,  this.font) &&
                 (defineEditText.height == this.height) &&
                 (defineEditText.color == this.color) &&
                 (defineEditText.maxLength == this.maxLength) &&
                 (defineEditText.align == this.align) &&
                 (defineEditText.leftMargin == this.leftMargin) &&
                 (defineEditText.rightMargin == this.rightMargin) &&
                 (defineEditText.ident == this.ident) &&
                 (defineEditText.leading == this.leading) &&
                 equals(defineEditText.varName, this.varName) &&
                 equals(defineEditText.initialText, this.initialText))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
