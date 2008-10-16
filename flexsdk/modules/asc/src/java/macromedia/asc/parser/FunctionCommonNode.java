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
import static macromedia.asc.parser.Tokens.*;

/**
 * Node
 *
 * @author Jeff Dyer
 */
public class FunctionCommonNode extends Node
{
	public FunctionDefinitionNode def;
	
	public IdentifierNode identifier;
	public FunctionSignatureNode signature;
	public StatementListNode body;
	public int fixedCount;
	public ObjectValue fun;
	public ReferenceValue ref;
	public ObjectList<FunctionCommonNode> fexprs;
	public int var_count;
	public int temp_count;
	public String internal_name = "";
	public ObjectList<Block> blocks;

    public int with_depth;

    public ObjectList<ObjectValue> scope_chain;

	public String debug_name = "";
	public int needsArguments;
	public Context cx;
	public int kind;
    public ObjectList<String> namespace_ids;
    public StatementListNode use_stmts;
    public DefaultXMLNamespaceNode default_dxns;
    public Multinames imported_names;

	public Namespaces used_namespaces;
	public ObjectValue private_namespace;
	public ObjectValue default_namespace;
	public ObjectValue public_namespace;

	private int flags;
	
	private static final int USER_DEFINED_BODY_Flag = 1;
	private static final int IS_FUNDEF_Flag         = 2;
	private static final int WITH_USED_Flag         = 4;
	private static final int IS_NATIVE_Flag         = 8;
	private static final int EXCEPTIONS_USED_Flag   = 16;
	
	public FunctionCommonNode(Context cx, StatementListNode use_stmts, String internal_name, IdentifierNode identifier, FunctionSignatureNode signature, StatementListNode body, boolean hasUserDefinedBody)
	{
		this.cx = cx;
		fun = null;
		ref = null;
		fexprs = null;
		var_count = 0;
		temp_count = 0;
		setNative(false);
		needsArguments = 0;
		kind = EMPTY_TOKEN;
		setFunctionDefinition(false);

        this.use_stmts = use_stmts;
        this.internal_name = internal_name;
		this.identifier = identifier;
		this.signature = signature;
		this.body = body;

		private_namespace = null;
		default_namespace = null;
		public_namespace = null;
		default_dxns = null;

        with_depth = -1;

        setUserDefinedBody(hasUserDefinedBody);
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

	public boolean isFunctionExpression()
	{
		return true;
	}

	public String toString()
	{
		return "FunctionCommon";
	}

	public void setFunctionDefinition(boolean is_fundef)
	{
		flags = is_fundef ? (flags|IS_FUNDEF_Flag) : (flags&~IS_FUNDEF_Flag);
	}

	public boolean isFunctionDefinition()
	{
		return (flags & IS_FUNDEF_Flag) != 0;
	}

	public void setUserDefinedBody(boolean userDefinedBody)
	{
		flags = userDefinedBody ? (flags|USER_DEFINED_BODY_Flag) : (flags&~USER_DEFINED_BODY_Flag);
	}

	public boolean isUserDefinedBody()
	{
		return (flags & USER_DEFINED_BODY_Flag) != 0;
	}

	public void setNative(boolean isNative)
	{
		flags = isNative ? (flags|IS_NATIVE_Flag) : (flags&~IS_NATIVE_Flag);
	}

	public boolean isNative()
	{
		return (flags & IS_NATIVE_Flag) != 0;
	}

	public void setWithUsed(boolean withUsed)
	{
		flags = withUsed ? (flags|WITH_USED_Flag) : (flags&~WITH_USED_Flag);
	}

	public boolean isWithUsed()
	{
		return (flags & WITH_USED_Flag) != 0;
	}

	public void setExceptionsUsed(boolean exceptionsUsed)
	{
		flags = exceptionsUsed ? (flags|EXCEPTIONS_USED_Flag) : (flags&~EXCEPTIONS_USED_Flag);
	}

	public boolean isExceptionsUsed()
	{
		return (flags & EXCEPTIONS_USED_Flag) != 0;
	}
}
