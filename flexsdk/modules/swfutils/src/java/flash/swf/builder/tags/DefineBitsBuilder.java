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

import flash.swf.tags.DefineBits;
import flash.swf.tags.DefineSprite;
import flash.swf.tags.DefineShape;
import flash.swf.tags.PlaceObject;
import flash.swf.Tag;
import flash.swf.types.TagList;
import flash.swf.types.Matrix;
import flash.graphics.images.JPEGImage;

import java.io.IOException;

/**
 * @author Paul Reilly
 * @author Peter Farland
 */
public class DefineBitsBuilder
{
	private DefineBitsBuilder()
	{
	}

	public static DefineBits build(JPEGImage image) throws IOException
	{
		DefineBits defineBits = new DefineBits(Tag.stagDefineBitsJPEG2);

		try
		{
			defineBits.data = image.getData();
		}
		finally
		{
			image.dispose();
		}

		return defineBits;
	}

    public static DefineBits build(String name, JPEGImage image)
    {
        DefineBits defineBits = null;
        try
        {
            defineBits = build(image);
            defineBits.name = name;
        }
        catch (IOException ex)
        {
            throw new RuntimeException("Error reading JPEG image " + image.getLocation() + ". " + ex.getMessage());
        }
        finally
        {
            image.dispose();
        }

        return defineBits;
    }

	public static DefineSprite buildSprite(String name, JPEGImage image)
	{
		TagList taglist = new TagList();

		try
		{
			DefineBits defineBits = build(image);
			taglist.defineBitsJPEG2(defineBits);

			DefineShape ds3 = ImageShapeBuilder.buildImage(defineBits, image.getWidth(), image.getHeight());
			taglist.defineShape3(ds3);

			PlaceObject po2 = new PlaceObject(ds3, 1);
			po2.setMatrix(new Matrix());
			// po2.setName(name);

			taglist.placeObject2(po2);
		}
		catch (IOException ex)
		{
			throw new RuntimeException("Error reading JPEG image " + image.getLocation() + ". " + ex.getMessage());
		}
		finally
		{
			image.dispose();
		}

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
