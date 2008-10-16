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
public class BevelFilter extends Filter
{
    public final static int ID = 3;
    public int getID() { return ID; }

    public int shadowColor;
    public int highlightColor;
    public int blurX;
    public int blurY;
    public int angle;
    public int distance;
    public int strength;
    public int flags;
}
