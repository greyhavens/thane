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

package flash.graphics.images;

import flash.util.Trace;
import flash.util.FileUtils;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.PixelGrabber;
import java.net.URL;
import java.net.URLConnection;
import java.net.MalformedURLException;

/**
 * @author Peter Farland
 */
public class LosslessImage extends BitmapImage
{
	public LosslessImage(File imageFile) throws MalformedURLException, IOException
	{
		this(imageFile.getName(), imageFile.toURL().openStream(), imageFile.lastModified());
    }

	public LosslessImage(String location, InputStream inputStream, long modified)
	{
        this.location = location;
		this.modified = modified;
        byte[] bytes = FileUtils.toByteArray(inputStream);
		Image image = ImageUtil.getImage(bytes);
        init(image);
	}

	public LosslessImage(URL imageURL)
	{
		Image image = Toolkit.getDefaultToolkit().getImage(imageURL);

		try
		{
			URLConnection conn = imageURL.openConnection();
			location = imageURL.toString();
			modified = conn.getLastModified();
		}
        catch (InternalError ie)
        {
            if (Trace.error)
            {
                ie.printStackTrace();
            }
            throw new InternalError("An error occurred because there is no graphics environment available.  Please set the headless-server setting in the configuration file to true.");
        }
        catch (NoClassDefFoundError ce)
        {
            if (Trace.error)
            {
                ce.printStackTrace();
            }
            throw new InternalError("An error occurred because there is no graphics environment available.  Please set the headless-server setting in the configuration file to true.");
        }
		catch (IOException ioe)
		{
			throw new RuntimeException("Error reading image from URL. " + ioe.getMessage());
		}

		init(image);
	}

	public LosslessImage(Image image)
	{
		location = "Synthetic";
		modified = System.currentTimeMillis();
		init(image);
	}

	private void init(Image image)
	{
        PixelGrabber pixelGrabber = ImageUtil.getPixelGrabber(image, location);

		width = pixelGrabber.getWidth();
		height = pixelGrabber.getHeight();
		Object p = pixelGrabber.getPixels();

		if (p != null)
		{
			Class ct = p.getClass().getComponentType();
			if (ct != null)
			{
				if (ct.equals(Integer.TYPE))
					pixels = (int[])p;
				else if (ct.equals(Byte.TYPE))
					throw new IllegalStateException("int[] of pixels expected, received byte[] instead.");
			}
		}
	}

	public int[] getPixels()
	{
		return pixels;
	}

	public void dispose()
	{

	}

	protected int[] pixels;
}
