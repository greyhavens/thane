////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package macromedia.asc.util;

import java.util.Set;
import java.util.Map;
import java.util.TreeMap;
import macromedia.asc.semantics.ObjectValue;
import static macromedia.asc.parser.Tokens.*;

/**
 * @author Jeff Dyer
 */
public final class Names
{
	private String[] names;
	private ObjectValue[] namespaces;
	private int[] bindings;
	private int size;
	private Names delegate;
	
	public static final int GET_NAMES          =  0x0;
	public static final int SET_NAMES          =  0x1;
	public static final int VAR_NAMES          =  0x2;
	public static final int METHOD_NAMES       =  0x3;
	public static final int LOCAL_METHOD_NAMES = 0x4;
	
	private static final int INITIAL_CAPACITY = 8;
	public static final Names EMPTY_NAMES = new Names();

	public Names()
	{
		delegate = EMPTY_NAMES;
	}
	
	public void setDelegate(Names names)
	{
		names.size();
		delegate = names;
	}

	private int hash(String name, int type)
	{
		assert name.intern() == name;
		return name.hashCode() * (type+1);
	}
	
	private int find(String name, int type, int m)
	{
        int n = 7;
		int bitmask = (m - 1);
        int i = hash(name, type) & bitmask;
        String k;
        while ((k = names[i]) != null && (k != name || type != (bindings[i]&7)))
        {
			i = (i + (n++)) & bitmask;			// quadratic probe
		}
        return i;
	}
	
	private int find(String name, ObjectValue namespace, int type, int m)
	{
        int n = 7;
		int bitmask = (m - 1);
        int i = hash(name, type) & bitmask;
        String k;
        String namespaceName = namespace.name;
        assert namespaceName == namespaceName.intern();
        while ((k = names[i]) != null && !(k == name && type == (bindings[i]&7) && namespaceName == namespaces[i].name
                && namespace.getNamespaceKind()== namespaces[i].getNamespaceKind() ))
        {
			i = (i + (n++)) & bitmask;			// quadratic probe
		}
        return i;
	}

	public int size()
	{
		if(this == EMPTY_NAMES)
			return 0;
		return size + delegate.size();
	}
	
	private boolean isFull()
	{ 
		// 0.80 load factor
		return 5*(size+1) >= capacity()*4;
	}
	
	private final int capacity()
	{
		return (names != null) ? names.length : 0;
	}

	private int put(String name, ObjectValue namespace, int type)
	{
		if (names == null)
		{
			names = new String[INITIAL_CAPACITY];
			namespaces = new ObjectValue[INITIAL_CAPACITY];
			bindings = new int[INITIAL_CAPACITY];			
		}
		
		if (isFull())
		{
			grow();
		}
		
		int i = find(name, namespace, type, capacity());
		if (name.equals(names[i]))
		{
			// This <name,ns,type> tuple is already in the table
		}
		else
		{
			// New table entry for this <name,ns,type> tuple
			size++;
			names[i] = name;
			namespaces[i] = namespace;
		}		
		return i;
	}
	
	public void putMask(String name, ObjectValue namespace, int type)
	{		
		int i = put(name, namespace, type);
		bindings[i] = bindings[i] | type;
	}
	
	public void put(String name, ObjectValue namespace, int type, int slot)
	{		
		int i = put(name, namespace, type);
		bindings[i] = (slot<<3) | type;
	}	

	public boolean containsKey(String name, int type)
	{
		if (this == EMPTY_NAMES)
		{
			return false;
		}
		
		boolean hasIt = false;
		if(names != null) {
			int i = find(name, type, capacity());
			hasIt = names[i] != null && names[i] == name && (bindings[i]&7) == type && (bindings[i]>>3) != -1;
		}
		if(!hasIt)
			hasIt = delegate.containsKey(name, type); 	
		return hasIt;
	}

	public int get(String name, ObjectValue namespace, int type)
	{
		if (this == EMPTY_NAMES)
		{
			return -1;
		}
		
		int index = -1;
		if(names != null) {
			int i = find(name, namespace, type, capacity());
			index = (names[i] == name && (bindings[i]&7) == type) ? (bindings[i]>>3) : -1;
		}
		if(index == -1)
			index = delegate.get(name, namespace, type);
		return index;
	}

	public Set<Map.Entry<String, Qualifiers>> entrySet(int type)
	{
		// ISSUE This method is going to be a performance hit
		// since it creates a copy of the entire map.
		// Callers should be adjusted to no longer use this method.
		TreeMap<String, Qualifiers> map = new TreeMap<String, Qualifiers>();
		for (int i=0, n=capacity(); i<n; i++)
		{
			if (names[i] != null && (bindings[i]&7) == type)
			{
				Qualifiers q = map.get(names[i]);
				if (q == null)
				{
					q = new Qualifiers();
					map.put(names[i], q);
				}
				q.put(namespaces[i], bindings[i]>>3);
			}
		}
		return map.entrySet();
	}
	
	public Qualifiers get(String name, int type)
	{
		if (this == EMPTY_NAMES)
		{
			return null;
		}
		
		Qualifiers q = null;

		int m = capacity();
        int n = 7;
		int bitmask = (m - 1);
        int i = hash(name, type) & bitmask;
        String k;
        while ((k = names[i]) != null)
        {
        	if (k == name && (bindings[i]&7) == type)
        	{
        		if (q == null)
        		{
        			q = new Qualifiers();
        		}        			
        		q.put(namespaces[i], bindings[i]>>3);
        	}
        	
			i = (i + (n++)) & bitmask;			// quadratic probe
		}
		
        if(q != null) {
        	Qualifiers q2 = delegate.get(name, type); 
        	if(q2 != null)
        		q.putAll(q2);
        } else
        	q = delegate.get(name, type);
        
		return q;
	}

	public void putAll(Names names)
	{
		if (names == null)
		{
			int capacity = names.capacity();
			this.names = new String[capacity];
			namespaces = new ObjectValue[capacity];
			bindings = new int[capacity];
		}

		for (int i=0, n=names.capacity(); i<n; i++)
		{
			if (names.names[i] != null)
			{
				put(names.names[i], names.namespaces[i], names.bindings[i]&7, names.bindings[i]>>3);
			}
		}	
	}

	public void putAll(Names names, int type)
	{
		if (names == null)
		{
			int capacity = names.capacity();
			this.names = new String[capacity];
			namespaces = new ObjectValue[capacity];
			bindings = new int[capacity];
		}

		for (int i=0, n=names.capacity(); i<n; i++)
		{
			if (names.names[i] != null && (names.bindings[i]&7) == type)
			{
				put(names.names[i], names.namespaces[i], names.bindings[i]&7, names.bindings[i]>>3);
			}
		}	
	}
	
	public boolean containsKey(String name, ObjectValue namespace, int type)
	{
		if (this == EMPTY_NAMES)
		{			
			return false;
		}
		
		boolean hasIt = false;
		if(names != null) {
			int i = find(name, namespace, type, capacity());
			hasIt = names[i] == name && (bindings[i]>>3) != -1;
		}
		if(!hasIt)
			hasIt = delegate.containsKey(name, namespace, type);
		return hasIt;
	}

	private void grow()
	{
		// double our table
		int capacity = capacity()*2;

		String oldNames[] = names;
		ObjectValue oldNamespaces[] = namespaces;
		int oldBindings[] = bindings;
		
		names = new String[capacity];
		namespaces = new ObjectValue[capacity];
		bindings = new int[capacity];
		
        for (int i=0, n=oldNames.length; i < n; i++)
        {
            String oldName = oldNames[i];
            if (oldName != null)
            {
                // inlined & simplified version of put()
                int j = find(oldName, oldNamespaces[i], oldBindings[i]&7, capacity);
                names[j] = oldName;
                namespaces[j] = oldNamespaces[i];
                bindings[j] = oldBindings[i];
			}
        }
	}
	
	public static int getTypeFromKind(int kind)	
	{
		switch (kind)
		{
		case GET_TOKEN:
			return GET_NAMES;
		case SET_TOKEN:
			return SET_NAMES;
		case VAR_TOKEN:
			return VAR_NAMES;
		default:
			return METHOD_NAMES;
		}	
	}

	public String getName(int i)
	{
		return names[i];
	}

	public ObjectValue getNamespace(int i)
	{
		return namespaces[i];
	}

	public int getSlot(int i)
	{
		return bindings[i]>>3;
	}

	public int getType(int i)
	{
		return bindings[i]&7;
	}

	public int hasNext(int index)
	{
		int n = capacity();
		for (; index < n; index++)
		{
			if (names[index] != null)
			{
				return index;
			}
		}
		return -1;
	}
}
