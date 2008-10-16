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

package flash.swf.tags;

import flash.swf.TagHandler;

/**
 * @author Brian Deitte
 */
public class CSMTextSettings extends DefineTag
{
    public CSMTextSettings()
    {
        super(stagCSMTextSettings);
    }

    public void visit(TagHandler h)
	{
   		h.csmTextSettings(this);
	}

    public boolean equals(Object object)
    {
        boolean isEqual = false;
        if (super.equals(object) &&  (object instanceof CSMTextSettings))
        {
            CSMTextSettings settings = (CSMTextSettings)object;
            if (textReference.equals(settings.textReference) &&
                styleFlagsUseSaffron == settings.styleFlagsUseSaffron &&
                gridFitType == settings.gridFitType &&
                thickness == settings.thickness &&
                sharpness == settings.sharpness)
            {
                isEqual = true;
            }
        }
        return isEqual;
    }

    public DefineTag textReference;
    public int styleFlagsUseSaffron; // 0 if off, 1 if on
    public int gridFitType; // 0 if none, 1 if pixel, 2 if subpixel
    public long thickness;
    public long sharpness;
}
