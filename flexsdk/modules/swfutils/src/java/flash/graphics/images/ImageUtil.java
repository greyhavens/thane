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

package flash.graphics.images;

import flash.util.Trace;

import java.awt.*;
import java.awt.image.PixelGrabber;
import java.awt.image.ImageObserver;

/**
 * Methods for getting images.
 *
 * @author Brian Deitte
 */
// FIXME: there is more duplication in JPEGImage/LosslessImage that could be added here
public class ImageUtil
{
    public static Image getImage(byte[] bytes)
    {
        Image image;
        try
		{
            image = Toolkit.getDefaultToolkit().createImage(bytes);
        }
        catch (InternalError ie)
        {
            if (Trace.error)
            {
                ie.printStackTrace();
            }
            throw new InternalError("An error occurred because there is no graphics environment available.  Please set the headless-server setting in the Flex configuration file to true.");
        }
        catch (NoClassDefFoundError ce)
        {
            if (Trace.error)
            {
                ce.printStackTrace();
            }
            throw new InternalError("An error occurred because there is no graphics environment available.  Please set the headless-server setting in the Flex configuration file to true.");
        }
        return image;
    }

    public static PixelGrabber getPixelGrabber(Image image, String location)
    {
        PixelGrabber pixelGrabber = new PixelGrabber(image, 0, 0, -1, -1, true);

        try
        {
            pixelGrabber.grabPixels();
        }
        catch (InterruptedException interruptedException)
        {
	        if (Trace.error)
	        {
		        interruptedException.printStackTrace();
	        }
            throw new RuntimeException("Failed to grab pixels for image " + location);
        }

	    if (((pixelGrabber.getStatus() & ImageObserver.WIDTH) == 0) ||
			((pixelGrabber.getStatus() & ImageObserver.HEIGHT) == 0))
	    {
		    throw new RuntimeException("Failed to grab pixels for image " + location);
	    }

        return pixelGrabber;
    }
}
