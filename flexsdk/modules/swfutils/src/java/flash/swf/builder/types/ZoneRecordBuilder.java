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

package flash.swf.builder.types;

import flash.fonts.FontFace;
import flash.swf.builder.tags.FontBuilder;
import flash.swf.tags.ZoneRecord;
import flash.util.Trace;

/**
 * A simple abstract class to decouple FlashType ZoneRecord construction from
 * FontBuilder.
 */
public class ZoneRecordBuilder
{
    private static final String DEFAULT_BUILDER = "flash.fonts.flashtype.FlashTypeZoneRecordBuilder";
    
    protected String fontAlias;
    protected FontBuilder fontBuilder;
    protected FontFace fontFace;

    protected ZoneRecordBuilder()
    {
    }

    public void setFontAlias(String alias)
    {
        fontAlias = alias;
    }

    public void setFontBuilder(FontBuilder builder)
    {
        fontBuilder = builder;
    }

    public void setFontFace(FontFace face)
    {
        fontFace = face;
    }

    /**
     * This no-op method returns an empty ZoneRecord. Subclasses should
     * override this method. 
     */
    public ZoneRecord build(int character)
    {
        // Return an empty Zone Record...
        ZoneRecord zoneRecord = new ZoneRecord();
        zoneRecord.numZoneData = 2;
        zoneRecord.zoneData = new long[] {0, 0};
        zoneRecord.zoneMask = 0;
        return zoneRecord;
    }

    public static ZoneRecordBuilder createInstance()
    {
        try
        {
            Class c = Class.forName(DEFAULT_BUILDER);
            ZoneRecordBuilder builder = (ZoneRecordBuilder)c.newInstance();
            return builder;
        }
        catch (Throwable t)
        {
            if (Trace.error)
                Trace.trace("ZoneRecordBuilder implementation not found '" + DEFAULT_BUILDER + "'");
        }

        return null;
    }
}
