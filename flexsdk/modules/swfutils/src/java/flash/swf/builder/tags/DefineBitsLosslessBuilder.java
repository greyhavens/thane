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

package flash.swf.builder.tags;

import flash.swf.tags.DefineBitsLossless;
import flash.swf.tags.DefineSprite;
import flash.swf.tags.DefineShape;
import flash.swf.tags.PlaceObject;
import flash.swf.tags.DefineTag;
import flash.swf.tags.DefineBits;
import flash.swf.TagValues;
import flash.swf.types.TagList;
import flash.swf.types.Matrix;
import flash.graphics.images.LosslessImage;

/**
 * @author Paul Reilly
 * @author Peter Farland
 */
public class DefineBitsLosslessBuilder
{
	private DefineBitsLosslessBuilder()
	{
	}

	public static DefineBitsLossless build(LosslessImage image)
	{
		DefineBitsLossless defineBitsLossless = new DefineBitsLossless(TagValues.stagDefineBitsLossless2);
		defineBitsLossless.format = 5;

		defineBitsLossless.width = image.getWidth();
		defineBitsLossless.height = image.getHeight();
		int[] pixels = image.getPixels();

		defineBitsLossless.data = new byte[pixels.length * 4];

		for (int i = 0; i < pixels.length; i++)
		{
			int offset = i * 4;
			int alpha = (pixels[i] >> 24) & 0xFF;
			defineBitsLossless.data[offset] = (byte)alpha;

			// [preilly] Ignore the other components if alpha is transparent.  This seems
			// to be a bug in the player.  Additionally, premultiply the alpha and the
			// colors, because the player expects this.
			if (defineBitsLossless.data[offset] != 0)
			{
				int red = (pixels[i] >> 16) & 0xFF;
				defineBitsLossless.data[offset + 1] = (byte)((red * alpha) / 255);
				int green = (pixels[i] >> 8) & 0xFF;
				defineBitsLossless.data[offset + 2] = (byte)((green * alpha) / 255);
				int blue = pixels[i] & 0xFF;
				defineBitsLossless.data[offset + 3] = (byte)((blue * alpha) / 255);
			}
		}

		return defineBitsLossless;
	}

    public static DefineBits build(String name, LosslessImage image)
    {
        DefineBitsLossless defineBits = build(image);
        defineBits.name = name;
        return defineBits;
    }

	public static DefineTag buildSprite(String name, LosslessImage image)
	{
		TagList taglist = new TagList();
		DefineBitsLossless defineBits = build(image);
		taglist.defineBitsLossless2(defineBits);

		DefineShape ds3 = ImageShapeBuilder.buildImage(defineBits, defineBits.width, defineBits.height);
		taglist.defineShape3(ds3);

		PlaceObject po2 = new PlaceObject(ds3, 1);
		po2.setMatrix(new Matrix());
		//Ahipo2.setName(name);

		taglist.placeObject2(po2);

		return defineSprite(name, taglist);
	}

	private static DefineSprite defineSprite(String name, TagList taglist)
	{
		DefineSprite defineSprite = new DefineSprite();
		defineSprite.framecount = 1;
		defineSprite.tagList = taglist;
		defineSprite.name = name;
		return defineSprite;
	}
}
