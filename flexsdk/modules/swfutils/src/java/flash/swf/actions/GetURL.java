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

package flash.swf.actions;

import flash.swf.ActionHandler;
import flash.swf.ActionConstants;

/**
 * @author Clement Wong
 */
public class GetURL extends flash.swf.Action
{
	public GetURL()
	{
		super(ActionConstants.sactionGetURL);
	}

    public void visit(ActionHandler h)
	{
		h.getURL(this);
	}

    /**
	 * the URL can be of any type, including an HTML file, an image, or another SWF movie.
	 */
	public String url;

	/**
	 * If this movie is playing in a browser, the url will be displayed in the
	 * browser frame given by this target string.  The special target names
	 * "_level0" and "_level1" are used to load another SWF movie into levels 0
	 * and 1 respectively.
	 */
	public String target;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof GetURL))
        {
            GetURL getURL = (GetURL) object;

            if ( equals(getURL.url, this.url) &&
                 equals(getURL.target, this.target))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
