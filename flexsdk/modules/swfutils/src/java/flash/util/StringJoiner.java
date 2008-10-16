////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.util;

import java.util.Collection;
import java.util.Iterator;
import java.util.Map;

/**
 * @author Gordon Smith
 */
public class StringJoiner
{
	public static String join(Object[] array, String separator,
    						  ItemStringer itemStringer)
    {
    	StringBuilder sb = new StringBuilder();
    	
    	int n = array.length;
    	for (int i = 0; i < n; i++)
    	{
    		Object itemObj = array[i];
    		String itemStr = itemStringer != null ?
    				   		 itemStringer.itemToString(itemObj) :
    				   		 itemObj.toString();
    		sb.append(itemStr);
    		if (separator != null && i < n - 1)
    		{
    			sb.append(separator);
    		}
    	}
    	
    	return sb.toString();
    }
    
    public static String join(Object[] array, String separator)
    {
    	return join(array, separator, null);
    }
    
    public static String join(Collection collection, String separator,
    						  ItemStringer itemStringer)
    {
    	StringBuilder sb = new StringBuilder();
    	
     	for (Iterator iter = collection.iterator(); iter.hasNext(); )
    	{
    		Object itemObj = iter.next();
    		String itemStr = itemStringer != null ?
    				   		 itemStringer.itemToString(itemObj) :
    				   		 itemObj.toString();
    		sb.append(itemStr);
    		if (separator != null && iter.hasNext())
    		{
    			sb.append(separator);
    		}
    	}
    	
    	return sb.toString();
    }
    
    public static String join(Collection collection, String separator)
    {
    	return join(collection, separator, null);
    }
    
    public interface ItemStringer
    {
    	public String itemToString(Object obj);
    }
    
    public static class ItemQuoter implements ItemStringer
    {
    	public String itemToString(Object obj)
    	{
    		return "\"" + obj.toString() + "\"";
    	}
    }
    
    public static class MapEntryItemWithColon implements ItemStringer
    {
    	public String itemToString(Object obj)
    	{
    		Map.Entry e = (Map.Entry)obj;
    		String key = e.getKey().toString();
    		String value = e.getValue().toString();
    		return key + ": " + value;
    	}
    }
    
    public static class MapEntryItemWithEquals implements ItemStringer
    {
    	public String itemToString(Object obj)
    	{
    		Map.Entry e = (Map.Entry)obj;
    		String key = e.getKey().toString();
    		String value = e.getValue().toString();
    		return key + "=\"" + value + "\"";
    	}
    }
}
