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
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.PixelGrabber;
import java.net.URL;
import java.net.URLConnection;

/**
 * @author Peter Farland
 */
public class JPEGImage extends BitmapImage
{
	public JPEGImage(String location, long modified, long length, InputStream inputStream)
        throws IOException
	{
        this.location = location;
        this.modified = modified;
        this.length = length;
        this.inputStream = inputStream;
        Image image = ImageUtil.getImage(getData());
    	init(image);
	}

	public JPEGImage(File imageFile)
	{
		try
		{
			location = imageFile.getAbsolutePath();
			modified = imageFile.lastModified();
			length = imageFile.length();
			inputStream = new FileInputStream(imageFile);

			Image image = Toolkit.getDefaultToolkit().getImage(location);

			init(image);
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
		catch (Exception ex)
		{
			throw new RuntimeException("Error reading image from file " + location + ". " + ex.getMessage());
		}
	}

	public JPEGImage(URL imageURL)
	{
		try
		{
			Image image = Toolkit.getDefaultToolkit().getImage(imageURL);
			inputStream = imageURL.openStream();

			URLConnection conn = imageURL.openConnection();
			location = imageURL.toString();
			modified = conn.getLastModified();
			length = conn.getContentLength();

			init(image);
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
		catch (Exception ex)
		{
			throw new RuntimeException("Error reading image from URL. " + location + ". " + ex.getMessage());
		}
	}

	public JPEGImage(InputStream is, long length)
	{
		inputStream = is;
		location = "Synthetic";
		modified = System.currentTimeMillis();
		this.length = length;
	}

	private void init(Image image)
	{
        PixelGrabber pixelGrabber = ImageUtil.getPixelGrabber(image, location);
		width = pixelGrabber.getWidth();
		height = pixelGrabber.getHeight();
	}

	public long getLength()
	{
		return length;
	}

	public byte[] getData() throws IOException
	{
		if (data == null)
		{
			data = FileUtils.toByteArray(inputStream, (int)length);
		}

		return data;
	}

	public void dispose()
	{
		try
		{
			inputStream.close();
		}
		catch (IOException ioe)
		{
		}
	}

	protected InputStream inputStream;
	protected long length;
	protected byte[] data;
}
