package macromedia.asc.parser;

import macromedia.asc.util.*;
import macromedia.asc.semantics.*;

/**
 * Node
 *
 * @author Dick Sweet
 */

public class UsePragmaNode extends Node {
	
	

	public Node identifier;
	public Node argument;

	public UsePragmaNode(Node idNode, Node argNode)
	{
		identifier = idNode;
		argument = argNode;
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

	public String toString()
	{
		return "UsePragma";
	}
}
