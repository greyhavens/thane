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

/*
 * Written by Jeff Dyer
 * Copyright (c) 1998-2003 Mountain View Compiler Company
 * All rights reserved.
 */

package macromedia.asc.parser;

import macromedia.asc.util.*;
import macromedia.asc.semantics.*;

/**
 * The base Node class.
 *
 * @author Jeff Dyer
 */
public class Node
{
	/* HACK -- JAVA ONLY
	 * Since a lot of code generation uses toString methods,
	 * we can't change the toString() methods in any nodes to something
	 * more useful for debugging.
	 * 	 Setting this to false leaves toStrings alone.
	 *   Setting this to true gives more useful toStrings, but breaks builds.
	 */
	public static final boolean useDebugToStrings = false;
	public static final int MAX_DEF_BITS = 1000; // ISSUE: what does this cost?
	
	/*private static HashMap<Class,Integer> nodecounts = new HashMap<Class,Integer>();
	public static void tally(Object o) {
		Class c = o.getClass();
		int i = nodecounts.containsKey(c) ? nodecounts.get(c) : 0;
		nodecounts.put(c, i+1);
	}
	static {
		Runtime.getRuntime().addShutdownHook(new Thread() {
			public void run() {
				TreeMap<Integer,Class> t = new TreeMap<Integer,Class>();
				for (Class c : nodecounts.keySet())
					t.put(nodecounts.get(c),c);
				for (int i : t.keySet())
					System.out.println(i + " " + t.get(i).getName());
			}
		});
	}*/
	
	protected int flags;
	public Block block;

	protected static final int IS_ANY_FLAG    = 1;
	protected static final int IS_ATTR_FLAG   = 2;
	protected static final int POSITION_SHIFT = 2;
	protected static final int POSITION_MASK  = 0xFFFFFFC; 
	
	public BitSet getGenBits()
	{
		return null;
	}

	public BitSet getKillBits()
	{
		return null;
	}

	public ReferenceValue getRef(Context cx)
	{
		return null;
	}

	public Node()
	{
		this(-1);
	}

	public Node(int position)
	{
		this.setPosition(position);
		block = null;
		//tally(this);
	}

	public Value evaluate(Context cx, Evaluator evaluator)
	{
		return null;
	}

	public Node first()
	{
		return this;
	}

	public Node last()
	{
		return this;
	}

	public Node pos(int p)
	{
		setPosition(p);
		return this;
	}

	public int pos()
	{
		return getPosition();
	}

	public String toString()
	{
		if(Node.useDebugToStrings)
         return "Node@" + pos();
      else
         return "Node";
	}

	public StringBuilder toCanonicalString(Context cx, StringBuilder buf)    
	{
		return buf;
	}

	public boolean isExpressionStatement()
	{
		return false;
	}

	public boolean isCallExpression()
	{
		return false;
	}

    public boolean isApplyTypeExpression()
    {
        return false;
    }

	public boolean isNewExpression()
	{
		return false;
	}

	public boolean isFullSuperExpression()
	{
		return false;
	}

	public boolean isMemberExpression()
	{
		return false;
	}

	public boolean isGetExpression()
	{
		return false;
	}

	public boolean isSetExpression()
	{
		return false;
	}

	public boolean isFunctionExpression()
	{
		return false;
	}

	public boolean isBooleanExpression()
	{
		return false;
	}

	public boolean isList()
	{
		return false;
	}

	public boolean isIdentifier()
	{
		return false;
	}

	public boolean isLabel()
	{
		return false;
	}

	public boolean isDefinition()
	{
		return false;
	}

	public boolean isConst()
	{
		return false;
	}

	public boolean isLabeledStatement()
	{
		return false;
	}

    public boolean isLiteral()
    {
        return false;
    }

	public boolean isLiteralNumber()
	{
		return false;
	}

	public boolean isLiteralInteger()
	{
		return false;
	}

	public boolean isStatementList()
	{
		return false;
	}

	public boolean isAttribute()
	{
		return false;
	}

	public boolean isDeleteExpression()
	{
		return false;
	}

	public boolean hasAttribute(String name)
	{
		return false;
	}

	public int countVars()
	{
		return 0;
	}

	public Node initializerStatement(Context cx)
	{
		return cx.getNodeFactory().emptyStatement();
	}

	public void setPositionNonterminal(Node expr)
	{
		setPositionNonterminal(expr, -1);
	}

	public void setPositionNonterminal(Node expr, int pos)
	{
		if (pos >= 0)
		{
			this.pos(pos);
		}
		else if (expr != null)
		{
			this.pos(expr.pos());
		}
		else
		{
			this.pos(0); // This is an internal error
		}

		if (this.getPosition() <= 0)
		{
            this.pos(0);  // ISSUE: should not get here
        }
	}

	public void setPositionTerminal(int pos)
	{
		if (pos >= 0)
		{
			this.pos(pos);
		}
		else
		{
			this.pos(0);
		}
	}

	public void setIsSynthetic() // set pos to -1 to indicate this is compiler generated helper code
	{
		this.pos(0);
	}
	public boolean inTerminalBlock() // This block has no successors due to a return or throw statement
	{
		return (block != null && block.succs.size() == 0);
	}

	public boolean isSynthetic()	// used to avoid warnings/errors for synthetic code
	{
		return (this.getPosition() == 0);
	}


	public void expectedType(TypeValue type)
	{
	}

	public void voidResult()
	{
	}

	public void setPosition(int position)
	{
        if( position < 0 )
        {
            position = 0;
        }
		flags &= ~POSITION_MASK;
		flags |= position << POSITION_SHIFT;
	}

	public int getPosition()
	{
		return (flags & POSITION_MASK) >> POSITION_SHIFT;
	}

    public boolean hasSideEffect()
    {
        return false;
    }
    
    public boolean isLValue()
    {
    	return false;
    }
    
    public boolean isConfigurationName()
    {
    	return false;
    }
}
