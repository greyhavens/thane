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

package flash.swf;

import java.io.IOException;

public class SwfFormatException extends IOException
{
    private static final long serialVersionUID = 7647895373289640210L;

    public SwfFormatException()
    {
    }

    public SwfFormatException(String s)
    {
        super(s);
    }
}
