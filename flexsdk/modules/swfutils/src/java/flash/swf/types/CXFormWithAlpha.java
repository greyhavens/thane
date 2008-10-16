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

package flash.swf.types;

import flash.swf.SwfEncoder;

/**
 * @author Clement Wong
 */
public class CXFormWithAlpha extends CXForm
{
    public int alphaMultTerm;
	public int alphaAddTerm;

    public int nbits()
    {
        // FFileWrite's MaxNum method takes only 4 arguments, so finding maximum value of 8 arguments takes three steps:
        int maxMult = SwfEncoder.maxNum(redMultTerm, greenMultTerm, blueMultTerm, alphaMultTerm);
        int maxAdd = SwfEncoder.maxNum(redAddTerm, greenAddTerm, blueAddTerm, alphaAddTerm);
        int maxValue = SwfEncoder.maxNum(maxMult, maxAdd, 0, 0);
        return SwfEncoder.minBits(maxValue, 1);
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (super.equals(object) && (object instanceof CXFormWithAlpha))
        {
            CXFormWithAlpha cxFormWithAlpha = (CXFormWithAlpha) object;

            if ( (cxFormWithAlpha.alphaMultTerm == this.alphaMultTerm) &&
                 (cxFormWithAlpha.alphaAddTerm == this.alphaAddTerm) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

}
