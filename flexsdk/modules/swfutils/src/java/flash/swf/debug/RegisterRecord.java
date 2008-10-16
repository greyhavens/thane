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

package flash.swf.debug;

import flash.swf.Action;
import flash.swf.ActionHandler;
import flash.swf.types.ActionList;

public class RegisterRecord extends Action
{
	public RegisterRecord(int offset, int numRegisters)
	{
		super(ActionList.sactionRegisterRecord);
		int size = numRegisters;
		registerNumbers = new int[size];
		variableNames = new String[size];
		this.offset = offset;
		next = 0;
	}

    public int[] registerNumbers;
	public String[] variableNames;
	public int offset;

	// internal use for addRegister()
	int next;

	/**
	 * Used to add a register entry into this record
	 */
	public void addRegister(int regNbr, String variableName)
	{
		registerNumbers[next] = regNbr;
		variableNames[next] = variableName;
		next++;
	}

	public int indexOf(int regNbr)
	{
		int at = -1;
		for(int i=0; at<0 && i<registerNumbers.length; i++)
		{
			if (registerNumbers[i] == regNbr)
				at = i;
		}
		return at;
	}

	public void visit(ActionHandler h)
	{
		h.registerRecord(this);
	}

    public String toString()
    {
		StringBuilder sb = new StringBuilder();
		sb.append(offset);
		sb.append(" ");
		for(int i=0; i<registerNumbers.length; i++)
			sb.append("$"+registerNumbers[i]+"='"+variableNames[i]+"' ");
		return ( sb.toString() ); 
    }

    public boolean equals(Object object)
    {
		boolean isIt = (object instanceof RegisterRecord); 
        if (isIt)
        {
            RegisterRecord other = (RegisterRecord) object;
            isIt = super.equals(other);
			for(int i=0; isIt && i<registerNumbers.length; i++)
			{
				isIt = ( (other.registerNumbers[i] == this.registerNumbers[i]) &&
						 (other.variableNames[i] == this.variableNames[i]) ) ? isIt : false;
			}
        }
		return isIt;
    }
}
