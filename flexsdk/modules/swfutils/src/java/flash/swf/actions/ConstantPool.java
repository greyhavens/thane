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

import java.util.Arrays;

import flash.swf.Action;
import flash.swf.ActionHandler;
import flash.swf.ActionConstants;

/**
 * @author Clement Wong
 */
public class ConstantPool extends Action
{
	public ConstantPool()
	{
		super(ActionConstants.sactionConstantPool);
	}

	public ConstantPool(String[] poolData)
	{
	    this();
		pool = poolData;
	}

    public void visit(ActionHandler h)
	{
		h.constantPool(this);
	}

    public String[] pool;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof ConstantPool))
        {
            ConstantPool constantPool = (ConstantPool) object;

            if ( Arrays.equals(constantPool.pool, this.pool) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        sb.append("ConstantPool[ pool = { ");
        for (int i = 0; i < pool.length; i++)
        {
            sb.append(pool[i]);
            sb.append(", ");
        }
        sb.append("} ]");
        return sb.toString();
    }
}
