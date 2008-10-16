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

package flash.swf.types;

import java.util.List;

/**
 * If you want to use ArrayList.toArray() so that you can use Arrays.equals,
 * please use this class.
 */
public class ArrayLists
{
	public static boolean equals(List a1, List a2)
	{
		if (a1 == a2)
		{
			return true;
		}

		if (a1 == null || a2 == null)
		{
			return false;
		}

		int length = a1.size();
		if (a2.size() != length)
		{
			return false;
		}

		for (int i = 0; i < length; i++)
		{
			Object o1 = a1.get(i);
			Object o2 = a2.get(i);
			if (!(o1 == null ? o2 == null : o1.equals(o2)))
			{
				return false;
			}
		}

		return true;
	}
}
