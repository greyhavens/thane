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

package macromedia.asc.parser;

import macromedia.asc.semantics.Value;
import macromedia.asc.util.Context;

/**
 * @author Clement Wong
 */
public class MetaDataNode extends Node
{
	public LiteralArrayNode data;

	public MetaDataNode(LiteralArrayNode data)
	{
		this.data = data;
		def = null;
	}

	public String id;
	public Value[] values = null;
	public DefinitionNode def;

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

	public String getValue(String key)
	{
		for (int i = 0, length = count(); i < length; i++)
		{
			if (values[i] instanceof MetaDataEvaluator.KeyValuePair)
			{
				if (((MetaDataEvaluator.KeyValuePair) values[i]).key.equals(key))
				{
					return ((MetaDataEvaluator.KeyValuePair) values[i]).obj;
				}
			}
		}
		return null;
	}

	public String getValue(int index)
	{
		if (index < 0 || index >= count())
		{
			throw new ArrayIndexOutOfBoundsException();
		}
		else if (values[index] instanceof MetaDataEvaluator.KeylessValue)
		{
			return ((MetaDataEvaluator.KeylessValue) values[index]).obj;
		}
		else if (values[index] instanceof MetaDataEvaluator.KeyValuePair)
		{
			return ((MetaDataEvaluator.KeyValuePair) values[index]).obj;
		}
		else
		{
			return null;
		}
	}

	public int count()
	{
		return values != null ? values.length : 0;
	}

	public String toString()
	{
		return "MetaData";
	}
}
