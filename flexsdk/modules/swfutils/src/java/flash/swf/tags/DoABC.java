////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2006 Adobe Systems Incorporated
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
public class DoABC extends Tag
{
    public DoABC(String name, int flag)
    {
        super(stagDoABC2);
        abc = new byte[0];
        this.name = name;
        this.flag = flag;
    }
	public DoABC()
	{
		super(stagDoABC);
		abc = new byte[0];
        name = null;
        flag = 1;
	}

	public void visit(TagHandler h)
	{
		h.doABC(this);
	}

	public byte[] abc;
    public String name;
    public int flag;

	public boolean equals(Object object)
	{
		boolean isEqual = false;

		if (super.equals(object) && (object instanceof DoABC))
		{
			DoABC doABC = (DoABC) object;

			if (equals(doABC.abc, this.abc) && equals(doABC.name, this.name) && doABC.flag == this.flag)
			{
				isEqual = true;
			}
		}

		return isEqual;
	}

	public int hashCode()
	{
		return super.hashCode() + DefineTag.PRIME * abc.hashCode() & name.hashCode() + flag;
	}
}
