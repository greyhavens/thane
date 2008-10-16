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

package flash.swf.types;

import flash.swf.types.GlyphEntry;
import junit.framework.TestCase;
import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author Paul Reilly
 */
public class GlyphEntryTest extends TestCase
{
    public GlyphEntryTest()
    {
        super("GlyphEntryTest");
    }

    public static Test suite()
    {
        return new TestSuite(GlyphEntryTest.class);
    }

    public void testClonePositive()
    {
        GlyphEntry glyphEntry1 = new GlyphEntry();
        glyphEntry1.setIndex(1);
        glyphEntry1.advance = 1;

        GlyphEntry glyphEntry2 = (GlyphEntry) glyphEntry1.clone();

        assertEquals("glyphEntry1 should be equal to glyphEntry2",
                     glyphEntry1, glyphEntry2);
    }

    public void testEqualsPositive()
    {
        GlyphEntry glyphEntry1 = new GlyphEntry();
        glyphEntry1.setIndex(1);
        glyphEntry1.advance = 1;

        GlyphEntry glyphEntry2 = new GlyphEntry();
        glyphEntry2.setIndex(1);
        glyphEntry2.advance = 1;

        assertEquals("glyphEntry1 should be equal to glyphEntry2",
                     glyphEntry1, glyphEntry2);
    }

    public void testEqualsNegative()
    {
        GlyphEntry glyphEntry1 = new GlyphEntry();
        glyphEntry1.setIndex(1);
        glyphEntry1.advance = 1;

        GlyphEntry glyphEntry2 = new GlyphEntry();
        glyphEntry2.setIndex(2);
        glyphEntry2.advance = 2;
        assertFalse("glyphEntry1 should not be equal to glyphEntry2",
                    glyphEntry1.equals(glyphEntry2));
    }

    public static void main(String args[])
    {
        GlyphEntryTest glyphEntryTest = new GlyphEntryTest();

        glyphEntryTest.testClonePositive();
        glyphEntryTest.testEqualsPositive();
        glyphEntryTest.testEqualsNegative();
    }
}
