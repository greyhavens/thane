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

import java.awt.Color;

/**
 * @author Peter Farland
 */
public class SwfUtils implements SwfConstants
{
    /**
     * Forces a value within the range of 0 to 255.
     *
     * @param raw the initial value
     * @return a value forced between 0 and 255
     */
    public static int applyRange255(int raw)
    {
        if (raw < 0)
            raw = 0;
        else if (raw > 255)
            raw = 255;

        return raw;
    }

    /**
     * @param color a color object
     * @return integer representation, as 0xAARRGGBB
     */
    public static int colorToInt(Color color)
    {
        return ((color.getBlue() & 0xFF)
                | ((color.getGreen() & 0xFF) << 8)
                | ((color.getRed() & 0xFF) << 16)
                | ((color.getAlpha() & 0xFF) << 24));
    }

    /**
     * @param r
     * @param g
     * @param b
     * @return 0x00RRGGBB
     */
    public static int colorToInt(int r, int g, int b)
    {
        return ((b & 0xFF)
                | ((g & 0xFF) << 8)
                | ((r & 0xFF) << 16));
    }

    /**
     * @param r
     * @param g
     * @param b
     * @param a
     * @return 0xAARRGGBB
     */
    public static int colorToInt(int r, int g, int b, int a)
    {
        return ((b & 0xFF)
                | ((g & 0xFF) << 8)
                | ((r & 0xFF) << 16)
                | ((a & 0xFF) << 24));
    }

    /**
     * @param i as 0xAARRGGBB
     * @return
     */
    public static Color intToColor(int i)
    {
        return new Color((i >> 16) & 0xFF, i >> 8 & 0xFF, i & 0xFF, i >> 24 & 0xFF);
    }

    public static int toTwips(double d)
    {
        return (int)Math.rint(d * TWIPS_PER_PIXEL);
    }

    public static double fromTwips(int d)
    {
        return d / TWIPS_PER_PIXEL;
    }

    public static String toUnicodePoint(char c)
    {
        StringBuilder sb = new StringBuilder(5);
        for (int i = 3; i >= 0; i--)
        {
            sb.append(Character.forDigit((c >> (4 * i)) & 15, 16));
        }
        return sb.toString();
    }
}
