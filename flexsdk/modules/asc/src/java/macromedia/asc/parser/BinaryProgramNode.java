////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2007 Adobe Systems Incorporated
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

import macromedia.asc.util.Context;
import macromedia.asc.semantics.Value;
import macromedia.asc.semantics.QName;

import java.util.List;
import java.util.ArrayList;

/**
 * @author Erik Tierney
 */
/**
 * Node
 *
 * @author Jeff Dyer
 */
public class BinaryProgramNode extends ProgramNode
{
	public BinaryProgramNode(Context cx, StatementListNode statements)
	{
        super( cx, statements );
	}

	public List<QName> toplevelDefinitions = new ArrayList<QName>();
	
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

	public String toString()
	{
		return "BinaryProgram";
	}
}
