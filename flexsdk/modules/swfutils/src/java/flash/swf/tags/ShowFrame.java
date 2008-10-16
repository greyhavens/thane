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

/**
 * @author Clement Wong
 */
public class ShowFrame extends Tag
{
	public ShowFrame()
	{
        super(stagShowFrame);
	}

	public void visit(TagHandler h)
	{
		h.showFrame(this);
	}
}
