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
import flash.swf.ActionFactory;
import flash.swf.ActionConstants;

/**
 * @author Clement Wong
 */
public class Push extends Action
{
    public Push()
	{
		super(ActionConstants.sactionPush);
	}

	public Push(Object value)
	{
		this();
		this.value = value;
	}

    public void visit(ActionHandler h)
	{
		h.push(this);
	}

    /** the value to push */
    public Object value;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof Push))
        {
            Push push = (Push) object;

            if ( equals(push.value, this.value))
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

	public static int getTypeCode(Object value)
	{
	    if (value == null) return kPushNullType;
	    if (value == ActionFactory.UNDEFINED) return kPushUndefinedType;
		if (value instanceof String)  return kPushStringType;
		if (value instanceof Float)  return kPushFloatType;
		if (value instanceof Byte)  return kPushRegisterType;
		if (value instanceof Boolean)  return kPushBooleanType;
		if (value instanceof Double)  return kPushDoubleType;
		if (value instanceof Integer)  return kPushIntegerType;
		if (value instanceof Short)
			return ((((Short)value).intValue() & 0xFFFF) < 256) ? kPushConstant8Type : kPushConstant16Type;
		assert (false);
	    return kPushStringType;
	}
}
