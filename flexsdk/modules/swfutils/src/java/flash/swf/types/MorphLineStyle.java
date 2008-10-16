////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.types;

/**
 * @author Clement Wong
 */
public class MorphLineStyle
{
    public int startWidth;
	public int endWidth;

	// MorphLineStyle2
	public int startCapsStyle;
	public int jointStyle;
	public boolean hasFill;
	public boolean noHScale;
	public boolean noVScale;
	public boolean pixelHinting;
	public boolean noClose;
	public int endCapsStyle;
	public int miterLimit;

	public MorphFillStyle fillType;
	// end MorphLineStyle2
	
    /** colors as ints: 0xAARRGGBB */
	public int startColor;
	public int endColor;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof MorphLineStyle)
        {
            MorphLineStyle morphLineStyle = (MorphLineStyle) object;

            if ( (morphLineStyle.startWidth == this.startWidth) &&
                 (morphLineStyle.endWidth == this.endWidth) &&
                 
                 (morphLineStyle.startCapsStyle == this.startCapsStyle) &&
                 (morphLineStyle.jointStyle == this.jointStyle) &&
                 (morphLineStyle.hasFill == this.hasFill) &&
                 (morphLineStyle.noHScale == this.noHScale) &&
                 (morphLineStyle.noVScale == this.noVScale) &&
                 (morphLineStyle.pixelHinting == this.pixelHinting) &&
                 (morphLineStyle.noClose == this.noClose) &&
                 (morphLineStyle.endCapsStyle == this.endCapsStyle) &&
                 (morphLineStyle.miterLimit == this.miterLimit) &&
                 morphLineStyle.fillType.equals(this.fillType) &&
                 
                 (morphLineStyle.startColor == this.startColor) &&
                 (morphLineStyle.endColor == this.endColor) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }    
}
