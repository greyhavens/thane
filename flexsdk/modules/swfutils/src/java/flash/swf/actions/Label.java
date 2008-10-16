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

import flash.swf.Action;
import flash.swf.ActionHandler;
import flash.swf.types.ActionList;

/**
 * @author Edwin Smith
 */
public class Label extends Action
{
    public Label()
    {
        super(ActionList.sactionLabel);
    }

    public void visit(ActionHandler h)
    {
        h.label(this);
    }

    public boolean equals(Object object)
    {
        // labels should always be unique unless they really are the same object
        return this == object;
    }

    public int hashCode()
    {
        // Action.hashCode() allways returns the code, but we want a real hashcode
        // since every instance of Label needs to be unique
        return super.objectHashCode();
    }
}
