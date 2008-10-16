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
 * The FrameLabel tag gives the specified Name to the current frame. This name is used by
 * ActionGoToLabel to identify the frame.
 *
 * Any frame can have a FrameLabel tag but only the main timeline
 * may have bookmark labels.  bookmark labels on sprite timelines
 * are ignored by the player.
 *
 * @author Clement Wong
 * @since SWF3
 */
public class FrameLabel extends Tag
{
	public FrameLabel()
	{
		super(stagFrameLabel);
	}

    public void visit(TagHandler h)
	{
		h.frameLabel(this);
	}

    /**
	 * label for the current frame
	 */
	public String label;

	/**
	 * named anchor flag.  labels this frame for seeking using HTML anchor syntax.
	 * @since SWF6
     */
	public boolean anchor;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof FrameLabel))
        {
            FrameLabel frameLabel = (FrameLabel) object;

            if ( equals(frameLabel.label, this.label) &&
                 (frameLabel.anchor == this.anchor) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
