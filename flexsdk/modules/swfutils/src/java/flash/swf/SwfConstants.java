////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf;

/**
 * @author Peter Farland
 */
public interface SwfConstants
{
    /**
     * Assumes a resolution of 72 dpi, at which the Macromedia Flash Player
     * renders 20 twips to a pixel.
     */
    int TWIPS_PER_PIXEL = 20;

    int WIDE_OFFSET_THRESHOLD = 65535;

    float FIXED_POINT_MULTIPLE = 65536.0F;
    float FIXED_POINT_MULTIPLE_8 = 256.0F;
    float MORPH_MAX_RATIO = 65535.0F;

    int GRADIENT_SQUARE = 32768;

    int LANGCODE_DEFAULT = 0;
    int LANGCODE_LATIN = 1;
    int LANGCODE_JAPANESE = 2;
    int LANGCODE_KOREAN = 3;
    int LANGCODE_SIMPLIFIED_CHINESE = 4;
    int LANGCODE_TRADIIONAL_CHINESE = 5;

    int TEXT_ALIGN_LEFT = 0;
    int TEXT_ALIGN_RIGHT = 1;
    int TEXT_ALIGN_CENTER = 2;
    int TEXT_ALIGN_JUSTIFY = 3;
}
