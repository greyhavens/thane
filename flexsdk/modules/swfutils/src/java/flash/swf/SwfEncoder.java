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

import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.zip.Deflater;
import java.util.zip.DeflaterOutputStream;

public class SwfEncoder extends RandomAccessBuffer
{
    private int bitPos = 8; //Must start as a full byte with value of 8
    private byte currentByte = 0x00;
    private int compressPos = -1;

    final int swfVersion;

    public SwfEncoder(int version)
    {
        super();
        this.swfVersion = version;
    }

    public void writeUI8(int c)
    {
        if (bitPos != 8 || c < 0 || c > 255)
        {
            assert (bitPos == 8);
            assert (c >= 0 && c <= 255) : ("UI8 overflow "+Integer.toHexString(c));
        }
        super.write(c);
    }

    public void writeFixed8(float v)
    {
        int f8 = ((int)(v*256)) & 0xffff;
        writeUI16(f8);
    }

    public void writeUI16(int c)
    {
        // FIXME - we should really deal with this upstream.
        // The standard case here is an unused fillstyle
        // when importing swfs with bitmaps from Matador.
        if (c == -1)
            c = 65535;
        assert (bitPos == 8);
        assert (c >= 0 && c <= 65535) : ("UI16 overflow");
        super.write(c);
        super.write(c >> 8);
    }

    public void writeSI16(int c)
    {
        assert (bitPos == 8);
        assert (c >= -32768 && c <= 32767) : ("SI16 overflow");
        super.write(c);
        super.write(c >> 8);
    }

    public void write32(int c)
    {
        assert (bitPos == 8);
        super.write(c);
        super.write(c >> 8);
        super.write(c >> 16);
        super.write(c >> 24);
    }

    public void write64(long c)
    {
        write32((int)(c));
        write32((int)(c>>32));
    }

    public void writeFloat(float f)
    {
        int i = Float.floatToIntBits( f );
        write32( i );
    }

    public void markComp()
    {
        compressPos = getPos();
    }

    /**
     * compress the marked section of our buffer, in place.
     * @throws IOException
     */
    public void compress() throws IOException
    {
        compress(false);
    }

    /**
     * compress the marked section of our buffer, in place.
     * @param isDebug If true, BEST_SPEED compression is used.  Otherwise, BEST_COMPRESSION is used.
     * @throws IOException
     */
    public void compress(boolean isDebug) throws IOException
    {
        if (compressPos != -1)
        {
            // compress in place from compressPos to pos
            pos = compressPos;
            deflate(this, isDebug);
            compressPos = -1;
        }
    }

    private void deflate(OutputStream out, boolean isDebug) throws IOException
    {
        int compression;

        if (isDebug)
        {
            compression = Deflater.BEST_SPEED;
        }
        else
        {
            compression = Deflater.BEST_COMPRESSION;
        }

        DeflaterOutputStream deflater = new DeflaterOutputStream(out, new Deflater(compression));

        deflater.write(buf, compressPos, count-compressPos);
        deflater.finish();
    }

    /**
     * send buffer to the given stream.  If markComp was called, bytes after that mark
     * will be compressed.
     * @param out
     * @throws IOException
     */
    public synchronized void writeTo(OutputStream out) throws IOException
    {
        writeTo(out, false);
    }

    /**
     * send buffer to the given stream.  If markComp was called, bytes after that mark
     * will be compressed.
     * @param out
     * @param isDebug If true, BEST_SPEED compression is used.  Otherwise, BEST_COMPRESSION is used.
     * @throws IOException
     */
    public synchronized void writeTo(OutputStream out, boolean isDebug) throws IOException
    {
        if (compressPos == -1)
        {
            super.writeTo(out);
        }
        else
        {
            count = pos;
            out.write(buf, 0, compressPos);
            deflate(out, isDebug);
        }
    }

    public void writeBit(boolean data)
    {
        writeBits(data ? 1 : 0, 1);
    }

    private void writeBits(int data, int size)
    {
//        if (print&&size>0) System.out.println("  write"+size+" "+data);
        while (size > 0)
        {
            if (size > bitPos)
            {
                //if more bits left to write than shift out what will fit
                currentByte |= data << (32 - size) >>> (32 - bitPos);

                // shift all the way left, then right to right
                // justify the data to be or'ed in
                super.write(currentByte);
                size -= bitPos;
                currentByte = 0;
                bitPos = 8;
            }
            else // if (size <= bytePos)
            {
                currentByte |= data << (32 - size) >>> (32 - bitPos);
                bitPos -= size;
                size = 0;

                if (bitPos == 0)
                {
                    //if current byte is filled
                    super.write(currentByte);
                    currentByte = 0;
                    bitPos = 8;
                }
            }
        }
    }

    public void writeUBits(int data, int size)
    {
        assert (data >= 0 && data <= (1<<size)-1);
        writeBits(data, size);
    }

    public void writeSBits(int data, int size)
    {
        assert (data >= -(1<<(size-1)) && data <= (1<<(size-1))-1);
        writeBits(data, size);
    }

    public void flushBits()
    {
        if (bitPos != 8)
        {
            super.write(currentByte);
            currentByte = 0;
            bitPos = 8;
        }
    }

    public synchronized void reset()
    {
        super.reset();
        compressPos = -1;
    }

    public void writeUI8at(int pos, int value)
    {
        int oldPos = getPos();
        setPos(pos);
        writeUI8(value);
        setPos(oldPos);
    }

    public void writeUI16at(int pos, int value)
    {
        int oldPos = getPos();
        setPos(pos);
        writeUI16(value);
        setPos(oldPos);
    }

    public void writeSI16at(int pos, int value)
    {
        int oldPos = getPos();
        setPos(pos);
        writeSI16(value);
        setPos(oldPos);
    }

    public void write32at(int pos, int value)
    {
        int oldPos = getPos();
        setPos(pos);
        write32(value);
        setPos(oldPos);
    }

    public void writeString(String s)
    {
        try
        {
            assert (bitPos == 8);
            write(swfVersion >= 6 ? s.getBytes("UTF8") : s.getBytes());
            write(0);
        }
        catch (UnsupportedEncodingException e)
        {
            assert (false);
        }
    }

    public void writeLengthString(String name)
    {
        try
        {
            assert (bitPos == 8);
            byte[] b = swfVersion >= 6 ? name.getBytes("UTF8") : name.getBytes();

            // [paul] Flash Authoring and the player expect the String
            // to be null terminated.
            super.write(b.length + 1);
            write(b);
            write(0);
        }
        catch (UnsupportedEncodingException e)
        {
            assert (false);
        }
    }


    /**
	 *  Compares the absolute values of 4 signed integers and returns the unsigned magnitude of
	 *  the number with the greatest absolute value.
    */
	public static int maxNum(int a, int b, int c, int d)
	{
		//take the absolute values of the given numbers
		a = Math.abs(a);
		b = Math.abs(b);
		c = Math.abs(c);
		d = Math.abs(d);

		//compare the numbers and return the unsigned value of the one with the greatest magnitude
        return a > b
                ? (a > c
                    ? (a > d ? a : d)
                    : (c > d ? c : d))
                : (b > c
                    ? (b > d ? b : d)
                    : (c > d ? c : d));
	}

    /**
     *  Calculates the minimum number of bits necessary to represent the given number.  The
	 *	number should be given in its unsigned form with the starting bits equal to 1 if it is
	 *	signed.  Repeatedly compares number to another unsigned int called x.
	 *	x is initialized to 1.  The value of x is shifted left i times until x is greater
	 *	than number.  Now i is equal to the number of bits the UNSIGNED value of number needs.
	 *	The signed value will need one more bit for the sign so i+1 is returned if the number
	 *	is signed, and i is returned if the number is unsigned.
     * @param number the number to compute the size of
     * @param bits 1 if number is signed, 0 if unsigned
     */
	public static int minBits(int number, int bits)
	{
          int val=1;
          for (int x = 1; val <= number; x <<=1) {
            val = val | x;
            bits++;
          }

        if (bits > 32)
		{
			assert false : ("minBits "+bits+" must not exceed 32");
		}
        return bits;
    }

    public void writeAt(int offset, byte[] b)
    {
        int oldPos = getPos();
        setPos(offset);
        write(b);
        setPos(oldPos);
    }
}
