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

package flash.swf;

import flash.swf.debug.DebugModule;
import flash.swf.debug.LineRecord;
import flash.swf.debug.RegisterRecord;
import flash.swf.types.FlashUUID;

/**
 * handler interface for SWD elements
 */
public interface DebugHandler
        extends DebugTags
{
    public void header(int version);

    public void uuid(FlashUUID id);
    public void module(DebugModule dm);
    public void offset(int offset, LineRecord lr);
	public void registers(int offset, RegisterRecord r);

    public void breakpoint(int offset);

	public void error(String message);
}
