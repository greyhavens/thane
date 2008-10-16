////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.localization;

import java.util.Locale;

/**
 * @author Roger Gonzalez
 */
public interface ILocalizer
{
    public ILocalizedText getLocalizedText( Locale locale, String id );
}
