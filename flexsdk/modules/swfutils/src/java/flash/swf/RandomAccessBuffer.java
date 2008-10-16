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

package flash.swf;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.io.IOException;

public class RandomAccessBuffer extends ByteArrayOutputStream
{
	protected int pos = 0;

    /**
     * Init an 1k buffer (BAOS default is 32 bytes, so this should be better)
     */
    public RandomAccessBuffer()
    {
        super(1024);
    }
    
    public RandomAccessBuffer(int bufferSize)
    {
        super(bufferSize);
    }
    
	final public int getPos()
	{
		return pos;
	}

	final public void setPos(int pos)
	{
		this.pos = pos;
	}

    // we override this so we can declare the signature w/out any exceptions
    final public synchronized void write(byte[] b)
    {
        this.write(b, 0, b.length);
    }

	final public synchronized void write(byte[] b, int off, int len)
	{
		if (pos > count)
		{
			byte[] zeros = new byte[pos - count];
			super.write(zeros, 0, zeros.length);
			pos = count;
		}
		else if (pos < count)
		{
			int overlap = Math.min(count - pos, len);
			System.arraycopy(b, off, buf, pos, overlap);
			pos += overlap;
			off += overlap;
			len -= overlap;
		}
		super.write(b, off, len);
		pos += len;
	}

	final public synchronized void readFully(byte[] bytes)
	{
		int len = bytes.length;
		if (pos+len > count)
		{
			throw new IndexOutOfBoundsException();
		}
		System.arraycopy(buf, pos, bytes, 0, len);
		pos += len;
	}

	final public synchronized void write(int b)
	{
		if (count > pos)
		{
			buf[pos++] = (byte) b;
		}
		else
		{
			super.write(b);
			pos++;
		}
	}

	public synchronized void reset()
	{
		super.reset();
		pos = 0;
	}

	public synchronized byte[] toByteArray()
	{
		count = pos;
		return super.toByteArray();
	}

	public synchronized void writeTo(OutputStream out) throws IOException
	{
		count = pos;
		super.writeTo(out);
	}

    public byte[] getByteArray()
    {
        return buf;
    }
}
