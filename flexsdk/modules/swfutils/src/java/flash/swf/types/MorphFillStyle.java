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

import java.util.Arrays;

import flash.swf.tags.DefineTag;

/**
 * @author Clement Wong
 */
public class MorphFillStyle
{
    public boolean hasBitmapId()
    {
        return type == 0x40 || type == 0x41;
    }

	public int type;
    /** colors as ints: 0xAARRGGBB */
	public int startColor;
	public int endColor;
	public Matrix startGradientMatrix;
	public Matrix endGradientMatrix;
	public MorphGradRecord[] gradRecords;
    public DefineTag bitmap;
	public Matrix startBitmapMatrix;
	public Matrix endBitmapMatrix;

	// MorphFillStyle for DefineMorphShape2
	public int ratio1, ratio2;
	
    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof MorphFillStyle)
        {
            MorphFillStyle morphFillStyle = (MorphFillStyle) object;

            if ( (morphFillStyle.type == this.type) &&
                 (morphFillStyle.startColor == this.startColor) &&
                 (morphFillStyle.endColor == this.endColor) &&
                 (morphFillStyle.ratio1 == this.ratio1) &&
                 (morphFillStyle.ratio2 == this.ratio2) &&
                 ( ( (morphFillStyle.startGradientMatrix == null) && (this.startGradientMatrix == null) ) ||
                   ( (morphFillStyle.startGradientMatrix != null) && (this.startGradientMatrix != null) &&
                     morphFillStyle.startGradientMatrix.equals(this.startGradientMatrix) ) ) &&
                 ( ( (morphFillStyle.endGradientMatrix == null) && (this.endGradientMatrix == null) ) ||
                   ( (morphFillStyle.endGradientMatrix != null) && (this.endGradientMatrix != null) &&
                     morphFillStyle.endGradientMatrix.equals(this.endGradientMatrix) ) ) &&
                 Arrays.equals(morphFillStyle.gradRecords, this.gradRecords) &&
                 ( ( (morphFillStyle.bitmap == null) && (this.bitmap == null) ) ||
                   ( (morphFillStyle.bitmap != null) && (this.bitmap != null) &&
                     morphFillStyle.bitmap.equals(this.bitmap) ) ) &&
                 ( ( (morphFillStyle.startBitmapMatrix == null) && (this.startBitmapMatrix == null) ) ||
                   ( (morphFillStyle.startBitmapMatrix != null) && (this.startBitmapMatrix != null) &&
                     morphFillStyle.startBitmapMatrix.equals(this.startBitmapMatrix) ) ) &&
                 ( ( (morphFillStyle.endBitmapMatrix == null) && (this.endBitmapMatrix == null) ) ||
                   ( (morphFillStyle.endBitmapMatrix != null) && (this.endBitmapMatrix != null) &&
                     morphFillStyle.endBitmapMatrix.equals(this.endBitmapMatrix) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
