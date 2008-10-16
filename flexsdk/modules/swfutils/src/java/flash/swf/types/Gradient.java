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

import java.util.Arrays;

/**
 * @author Roger Gonzalez
 */
public class Gradient
{
    public static final int SPREAD_MODE_PAD = 0;
    public static final int SPREAD_MODE_REFLECT = 1;
    public static final int SPREAD_MODE_REPEAT = 2;
    public static final int INTERPOLATION_MODE_NORMAL = 0;
    public static final int INTERPOLATION_MODE_LINEAR = 1;

    public int spreadMode;
    public int interpolationMode;
    public GradRecord[] records;

    public boolean equals(Object o)
    {
        if (!(o instanceof Gradient))
            return false;

        Gradient otherGradient = (Gradient) o;
        return ((otherGradient.spreadMode == spreadMode)
                && (otherGradient.interpolationMode == interpolationMode)
                && Arrays.equals(otherGradient.records, records));

    }
}
