////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.fonts;

import junit.framework.TestCase;
import flash.swf.Tag;
import flash.swf.builder.tags.FontBuilder;

import java.awt.GraphicsEnvironment;

/**
 * A simple test to check that the FontManager is caching font loading and glyph generation by
 * checking that a second call takes less than 10% of the initial load time.
 *
 * @author Peter Farland
 */
public class FontCacheTest extends TestCase
{
    private String family;
    private FontManager fontManager;

    public FontCacheTest(String test)
    {
        super(test);
    }

    protected void setUp() throws Exception
    {
        fontManager = new JREFontManager();

        //Find a local font on the System...
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        String[] allFonts = ge.getAvailableFontFamilyNames();

        //Choose a random font name
        int i = 0;
        while (family == null && i < allFonts.length)
        {
            int randomIndex = (int)Math.rint((allFonts.length - 1) * Math.random());
            family = allFonts[randomIndex];
            i++;
        }

        //Nullify JIT considerations by running a dummy test
        try
        {
            //FontBuilder fontBuilder = new FontBuilder(Tag.stagDefineFont2, fontManager, "Nothing", "", 0, true);
        }
        catch (Throwable t)
        {
        }
    }

    protected long time() throws Throwable
    {
        long start = System.currentTimeMillis();

        FontBuilder fontBuilder = new FontBuilder(Tag.stagDefineFont2, fontManager, "TemporaryFont", family, 0, true, false);
        fontBuilder.addCharset("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcedefghijklmnopqrstuvwxyz".toCharArray());
        fontBuilder.build();

        return System.currentTimeMillis() - start;
    }

    protected void tearDown() throws Exception
    {
        fontManager = null;
        family = null;
    }


    public void testCache() throws Throwable
    {
        //Capture initial load time...
        long initialTime = time();

        //Test against second call... should be less than 10% of initial load time!
        long secondTime = time();

        assertFalse("FontCacheTest for " + family + " ran slower than 10% of initial load time! Initial time: " + initialTime + "ms, Second time: " + secondTime + "ms. Failed.",
                secondTime > (initialTime * 0.1));
    }

    protected void runTest() throws Throwable
    {
        setUp();

        testCache();

        tearDown();
    }

    public static void main(String[] args)
    {
        FontCacheTest test = new FontCacheTest("Font Cache Test");

        try
        {
            test.runTest();
        }
        catch (Throwable t)
        {
            t.printStackTrace();
        }
    }
}
