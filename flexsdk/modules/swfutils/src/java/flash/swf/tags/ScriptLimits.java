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
import flash.swf.TagValues;

/**
 * ScriptLimits.  Change the player's default scripting limits. This
 * tag applies until the next ScriptLimits is encountered at runtime;
 * it can occur anywhere and any number of times.
 *
 * @since SWF7
 *
 * @author Paul Reilly
 */

public class ScriptLimits extends Tag
{
    public ScriptLimits(int scriptRecursionLimit, int scriptTimeLimit)
    {
        super(TagValues.stagScriptLimits);

        this.scriptRecursionLimit = scriptRecursionLimit;
        this.scriptTimeLimit = scriptTimeLimit;
    }

    public void visit(TagHandler tagHandler)
	{
        tagHandler.scriptLimits(this);
	}

	public int scriptRecursionLimit;
    public int scriptTimeLimit;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof ScriptLimits))
        {
            ScriptLimits scriptLimits = (ScriptLimits) object;

            if ( (scriptLimits.scriptRecursionLimit == this.scriptRecursionLimit) &&
                 (scriptLimits.scriptTimeLimit == this.scriptTimeLimit) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
