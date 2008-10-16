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

import java.util.*;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

/**
 * @author Roger Gonzalez
 */
public class LocalizationManager
{
    public LocalizationManager()
    {

    }

    public void addLocalizer( ILocalizer localizer )
    {
        localizers.add( localizer );
    }

    private ILocalizedText getLocalizedTextInner( Locale locale, String id )
    {
        for (Iterator it = localizers.iterator(); it.hasNext(); )
        {
            ILocalizer localizer = (ILocalizer) it.next();

            ILocalizedText text = localizer.getLocalizedText( locale, id );

            if (text != null)
            {
                return text;
            }
        }

        return null;
    }

    private ILocalizedText getLocalizedText( Locale locale, String id )
    {
        ILocalizedText t = getLocalizedTextInner( locale, id );

        if ((t == null) && (locale.getCountry().length() > 0) && (locale.getVariant().length() > 0))
        {
            t = getLocalizedTextInner( new Locale( locale.getLanguage(), locale.getCountry() ), id );
        }

        if ((t == null) && (locale.getCountry().length() > 0))
        {
            t = getLocalizedTextInner( new Locale( locale.getLanguage() ), id );
        }

        return t;
    }

    protected static String replaceInlineReferences( String text, Map parameters )
    {
        if (parameters == null)
            return text;

        int depth = 100;
        while (depth-- > 0)
        {
            int o = text.indexOf( "${" );
            if (o == -1)
                break;
            if ((o >= 1) && (text.charAt( o-1 ) == '$'))
            {
                o = text.indexOf( "${", o+2 );
                if (o == -1)
                    break;
            }

            int c = text.indexOf( "}", o );

            if (c == -1)
            {
                return null; // FIXME
            }
            String name = text.substring( o + 2, c );
            String value = null;
            if (parameters.containsKey( name ) && (parameters.get( name ) != null))
            {
                value = parameters.get( name ).toString();
            }

            if (value == null)
            {
                value = "";
            }
            text = text.substring( 0, o ) + value + text.substring( c + 1 );
        }
        return text.replaceAll( "[$][$][{]", "\\${" );
    }

	public String getLocalizedTextString( String id )
	{
		return getLocalizedTextString( id, Collections.EMPTY_MAP );
	}

	public String getLocalizedTextString( String id, Map parameters )
	{
		return getLocalizedTextString( Locale.getDefault(), id, parameters );
	}

    public String getLocalizedTextString( Locale locale, String id, Map parameters )
    {
        ILocalizedText t = getLocalizedText( locale, id );

        if ((t == null) && !locale.equals( Locale.getDefault() ))
        {
            t = getLocalizedText( Locale.getDefault(), id );
        }
        if ((t == null) && !Locale.getDefault().getLanguage().equals( "en" ))
        {
            t = getLocalizedText( new Locale( "en" ), id );
        }

        return (t == null)? null : t.format( parameters );
    }

	public String getLocalizedTextString( Object object )
	{
		String s = getLocalizedTextString( Locale.getDefault(), object );

        return s;
    }

    // todo - this is a pretty specialized helper function, hoist up to client code?
    public String getLocalizedTextString( Locale locale, Object object )
    {
        String id = object.getClass().getName().replaceAll( "\\$", "." );

        Map<String, Object> parameters = new HashMap<String, Object>();
        Class c = object.getClass();

        while (c != Object.class)
        {
            Field[] fields = c.getDeclaredFields();

            for (int i = 0; i < fields.length; ++i)
            {
                Field f = fields[i];

                if (!Modifier.isPublic( f.getModifiers() ))
                {
                    continue;
                }
                if (Modifier.isStatic( f.getModifiers() ))
                {
                    continue;
                }

                try
                {
                    parameters.put( f.getName(), f.get( object ) );
                }
                catch (Exception e)
                {
                }
            }
            c = c.getSuperclass();
        }

        String s = null;
        if ((parameters.containsKey( "id" ) && parameters.get( "id" ) != null ))
        {
            String subid = parameters.get( "id" ).toString();
            if (subid.length() > 0)
            {
                // fixme - Formalize?
                s = getLocalizedTextString( locale, id + "." + subid, parameters );
            }
        }
        if (s == null)
        {
            s = getLocalizedTextString( locale, id, parameters );
        }

        if (s == null)
        {
            s = id;

            if (parameters != null)
            {
                s += "[";
                for (Iterator it = parameters.entrySet().iterator(); it.hasNext(); )
                {
                    Map.Entry e = (Map.Entry) it.next();
                    s += e.getKey();
                    if (e.getValue() != null)
                        s += "='" + e.getValue() + "'";
                    if (it.hasNext())
                        s += ", ";
                }
                s += "]";
            }
            return s;
        }

        return s;
    }

    private List<ILocalizer> localizers = new LinkedList<ILocalizer>();
}
