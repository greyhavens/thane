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
import flash.swf.types.ActionList;

/**
 * @author Clement Wong
 */
public class DoAction extends Tag
{
	public DoAction()
	{
		super(stagDoAction);
	}

    public DoAction(ActionList actions)
    {
        super(stagDoAction);
        this.actionList = actions;
    }

    public void visit(TagHandler h)
	{
		h.doAction(this);
    }

    public ActionList actionList;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof DoAction))
        {
            DoAction doAction = (DoAction) object;

            if ( equals(doAction.actionList, this.actionList) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int hashCode() {
      int hashCode = super.hashCode();
      hashCode += DefineTag.PRIME * actionList.size();
      return hashCode;
    }

}
