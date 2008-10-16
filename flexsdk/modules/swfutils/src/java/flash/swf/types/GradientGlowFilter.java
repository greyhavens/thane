////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.types;

/**
 * @author Roger Gonzalez
 */
public class GradientGlowFilter extends GlowFilter
{
    public final static int ID = 4;
    public int getID() { return ID; }

    public int numcolors;
    public int[] gradientColors;
    public int[] gradientRatio;
    public int angle;
    public int distance;
}
