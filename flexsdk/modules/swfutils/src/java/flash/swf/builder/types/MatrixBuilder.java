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

import flash.swf.SwfConstants;
import flash.swf.types.Matrix;

import java.awt.geom.AffineTransform;

/**
 * @author Peter Farland
 */
public final class MatrixBuilder
{
    private MatrixBuilder()
    {
    }

    public static Matrix build(AffineTransform at)
    {
        Matrix matrix = new Matrix();
        matrix.scaleX = (int)Math.rint(at.getScaleX() * SwfConstants.FIXED_POINT_MULTIPLE);
        matrix.scaleY = (int)Math.rint(at.getScaleY() * SwfConstants.FIXED_POINT_MULTIPLE);
        if (matrix.scaleX != 0 || matrix.scaleY != 0)
            matrix.hasScale = true;

        matrix.rotateSkew0 = (int)Math.rint(at.getShearY() * SwfConstants.FIXED_POINT_MULTIPLE); //Yes, these are supposed
        matrix.rotateSkew1 = (int)Math.rint(at.getShearX() * SwfConstants.FIXED_POINT_MULTIPLE); //to be flipped
        if (matrix.rotateSkew0 != 0 || matrix.rotateSkew1 != 0)
        {
            matrix.hasRotate = true;
            if ((at.getType() & AffineTransform.TYPE_MASK_ROTATION) != 0)
                matrix.hasScale = true; //A rotation operation in Flash requires both rotate and scale components, even if zero scale.
        }

        matrix.translateX = (int)Math.rint(at.getTranslateX() * SwfConstants.TWIPS_PER_PIXEL);
        matrix.translateY = (int)Math.rint(at.getTranslateY() * SwfConstants.TWIPS_PER_PIXEL);

        return matrix;
    }

    public static Matrix getTranslateInstance(double tx, double ty)
    {
        Matrix matrix = new Matrix();
        matrix.translateX = (int)Math.rint(tx * SwfConstants.TWIPS_PER_PIXEL);
        matrix.translateY = (int)Math.rint(ty * SwfConstants.TWIPS_PER_PIXEL);
        return matrix;
    }

}
