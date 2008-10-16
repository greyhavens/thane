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
public class CXForm
{
    public boolean hasAdd;
	public boolean hasMult;

	public int redMultTerm;
	public int greenMultTerm;
	public int blueMultTerm;
	public int redAddTerm;
	public int greenAddTerm;
	public int blueAddTerm;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof CXForm)
        {
            CXForm cxForm = (CXForm) object;

            if ( (cxForm.hasAdd == this.hasAdd) &&
                 (cxForm.hasMult == this.hasMult) &&
                 (cxForm.redMultTerm == this.redMultTerm) &&
                 (cxForm.greenMultTerm == this.greenMultTerm) &&
                 (cxForm.blueMultTerm == this.blueMultTerm) &&
                 (cxForm.redAddTerm == this.redAddTerm) &&
                 (cxForm.greenAddTerm == this.greenAddTerm) &&
                 (cxForm.blueAddTerm == this.blueAddTerm) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

	public String toString()
	{
		return redMultTerm + "r" + (redAddTerm>=0 ? "+" : "") + redAddTerm + " " +
				greenMultTerm + "g" + (greenAddTerm>=0 ? "+" : "") + greenAddTerm + " " +
				blueMultTerm + "b" + (blueAddTerm>=0 ? "+" : "") + blueAddTerm;
	}

    public int nbits()
    {
        // two step process to find maximum value of 6 numbers because "FSWFStream::MaxNum" takes only 4 arguments
        int max = SwfEncoder.maxNum(redMultTerm, greenMultTerm, blueMultTerm, redAddTerm);
        max = SwfEncoder.maxNum(greenAddTerm, blueAddTerm, max, 0);
        return SwfEncoder.minBits(max, 1);
    }
}
