////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.localization;

import java.util.Locale;
import java.util.ResourceBundle;
import java.util.Map;
import java.util.MissingResourceException;

/**
 * @author Roger Gonzalez
 */
public class ResourceBundleLocalizer implements ILocalizer
{
    public ILocalizedText getLocalizedText( Locale locale, String id )
    {
        String prefix = id;

        while (true)
        {
            int dot = prefix.lastIndexOf( '.' );
            if (dot == -1)
            {
                break;
            }
            prefix = prefix.substring( 0, dot );
            String suffix = id.substring( dot + 1 );
            try
            {
                ResourceBundle bundle = ResourceBundle.getBundle( prefix, locale );

                if ((bundle != null) && (bundle.getObject( suffix ) != null))
                {
                    return new ResourceBundleText( bundle.getObject( suffix ).toString() );
                }
            }
            catch (MissingResourceException e)
            {
            }
        }

        return null;
    }

    private class ResourceBundleText implements ILocalizedText
    {
        public ResourceBundleText( String text )
        {
            this.text = text;
        }
        public String format( Map parameters )
        {
            return LocalizationManager.replaceInlineReferences( text, parameters );
        }
        private String text;
    }
}
