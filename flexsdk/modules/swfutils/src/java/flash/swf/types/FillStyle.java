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

import flash.swf.tags.DefineTag;

/**
 * @author Clement Wong
 */
public class FillStyle
{
    public static final int FILL_SOLID = 0;

    public static final int FILL_GRADIENT = 0x10;
    public static final int FILL_LINEAR_GRADIENT = 0x10;
    public static final int FILL_RADIAL_GRADIENT = 0x12;
    public static final int FILL_FOCAL_RADIAL_GRADIENT = 0x13;

    public static final int FILL_VECTOR_PATTERN = 0x20;
    public static final int FILL_RAGGED_CROSSHATCH = 0x20;
    public static final int FILL_DIAGONAL_LINES = 0x21;
    public static final int FILL_CROSSHATCHED_LINES = 0x22;
    public static final int FILL_STIPPLE = 0x23;

    public static final int FILL_BITS = 0x40;
    public static final int FILL_BITS_CLIP = 0x01; // set if bitmap is clipped. otherwise repeating
    public static final int FILL_BITS_NOSMOOTH = 0x02; // set if bitmap should not be smoothed

    public FillStyle()
    {
    }

    public FillStyle(int type, Matrix matrix, DefineTag bitmap)
    {
        setType(type);
        this.matrix = matrix;
        this.bitmap = bitmap;
    }

    public FillStyle(int color)
    {
        this.type = FILL_SOLID;
        this.color = color;
    }

	public int type;

    /** color as int: 0xAARRGGBB or 0x00RRGGBB */
	public int color;
	public Gradient gradient;
    public Matrix matrix;
    public DefineTag bitmap;

    public int getType()
    {
        return type;
    }

    public boolean hasBitmapId()
    {
        return ((type & FILL_BITS) != 0);
    }

    public void setType(int type)
    {
        this.type = type;
		assert ((type == FILL_SOLID) ||
                (type == FILL_GRADIENT) ||
                (type == FILL_LINEAR_GRADIENT) ||
                (type == FILL_RADIAL_GRADIENT) ||
                (type == FILL_VECTOR_PATTERN) ||
                (type == FILL_RAGGED_CROSSHATCH) ||
                (type == FILL_STIPPLE) ||
                (type == FILL_BITS) ||
                (type == (FILL_BITS | FILL_BITS_CLIP)) ||
                (type == (FILL_BITS | FILL_BITS_NOSMOOTH)) ||
                (type == (FILL_BITS | FILL_BITS_NOSMOOTH | FILL_BITS_CLIP))   
                );
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof FillStyle)
        {
            FillStyle fillStyle = (FillStyle) object;

            if ( (fillStyle.type == this.type) &&
                 (fillStyle.color == this.color) &&
                 ( ( (fillStyle.gradient == null) && (this.gradient == null) ) ||
                     (fillStyle.gradient.equals( this.gradient )) ) &&
                 ( ( (fillStyle.matrix == null) && (this.matrix == null) ) ||
                   ( (fillStyle.matrix != null) && (this.matrix != null) &&
                     fillStyle.matrix.equals(this.matrix) )) &&
                 ( ( (fillStyle.bitmap == null) && (this.bitmap == null) ) ||
                   ( (fillStyle.bitmap != null) && (this.bitmap != null) &&
                     fillStyle.bitmap.equals(this.bitmap) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }
}
