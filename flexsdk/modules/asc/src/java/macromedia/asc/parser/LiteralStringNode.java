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
 * Node
 *
 * @author Jeff Dyer
 */
public class LiteralStringNode extends Node
{
	public String value;

	// consts used to identify string delimiter type.
	private static final int SINGLE_QUOTE_DELIMITER = 2;
	private static final int DOUBLE_QUOTE_DELIMITER = 1;
	private static final int OTHER_DELIMITER=0; // this can occur in an xml literal expression, or in LiteralStringNodes synthesized by the compiler
	
	private int delimiterType; // one of the above delim types
	
	public boolean isSingleQuote()
	{
		return delimiterType == SINGLE_QUOTE_DELIMITER;
	}

	public boolean isDoubleQuote()
	{
		return delimiterType == DOUBLE_QUOTE_DELIMITER;
	}

	public LiteralStringNode(String value)
	{
		void_result = false;
		this.value = value.intern();
		delimiterType = OTHER_DELIMITER;
	}

	public LiteralStringNode(String value, boolean singleQuoted)
	{
		this(value, singleQuoted, true);
	}

	/**
	 * This constructor is used by Flex direct AST generation.
	 *
	 * @param intern Controls whether value will be interned.  If
	 *				 <code>value</code> is an interned constant,
	 *				 <code>intern</code> should be false.  Otherwise,
	 *				 it should be true.
	 */
	public LiteralStringNode(String value, boolean singleQuoted, boolean intern)
	{
		void_result = false;

		if (intern)
		{
			this.value = value.intern();
		}
		else
		{
			assert value.intern() == value;
			this.value = value;
		}

		delimiterType = singleQuoted ? SINGLE_QUOTE_DELIMITER : DOUBLE_QUOTE_DELIMITER;
	}

	public Value evaluate(Context cx, Evaluator evaluator)
	{
		if (evaluator.checkFeature(cx, this))
		{
			return evaluator.evaluate(cx, this);
		}
		else
		{
			return null;
		}
	}

    public boolean isLiteral()
    {
        return true;
    }

	public boolean void_result;

	public void voidResult()
	{
		void_result = true;
	}

	public String toString()
	{
		return "LiteralString";
	}
}
