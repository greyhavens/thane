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

package flash.swf.debug;

import flash.swf.DebugHandler;
import flash.swf.types.FlashUUID;
import flash.util.IntMap;

/**
 * info gleaned from a debuggable flash movie (SWF+SWD)
 * @author Edwin Smith
 */
public class DebugTable
        implements DebugHandler
{
    public FlashUUID uuid;
    public int version;
    public IntMap lines;
	public IntMap registers;

    public DebugTable()
    {
        lines = new IntMap();
        registers = new IntMap();
    }

    public void breakpoint(int offset)
    {
    }

    public void header(int version)
    {
        this.version = version;
    }

    public void module(DebugModule dm)
    {
    }

    public void offset(int offset, LineRecord lr)
    {
        lines.put(offset, lr);
    }

    public void registers(int offset, RegisterRecord r)
    {
        registers.put(offset, r);
    }

    public void uuid(FlashUUID id)
    {
        this.uuid = id;
    }

    public LineRecord getLine(int offset)
    {
        return (LineRecord) lines.get(offset);
    }

    public RegisterRecord getRegisters(int offset)
    {
        return (RegisterRecord) registers.get(offset);
    }

	public void error(String msg)
	{
	}
}
