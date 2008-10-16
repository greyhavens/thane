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

package flash.swf.types;

/**
 * @author Roger Gonzalez
 */
public class FocalGradient extends Gradient
{
    public float focalPoint;

    public boolean equals(Object o)
    {
        if (!(o instanceof FocalGradient) || !super.equals(o))
            return false;

        FocalGradient otherGradient = (FocalGradient) o;
        return (otherGradient.focalPoint == focalPoint);
    }
}
