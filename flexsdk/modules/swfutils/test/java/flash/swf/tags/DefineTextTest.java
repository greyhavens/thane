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

package flash.swf.tags;

import flash.swf.tags.DefineText;
import junit.framework.TestCase;
import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author Paul Reilly
 */
public class DefineTextTest extends TestCase
{
    public DefineTextTest()
    {
        super("DefineTextTest");
    }

    public static Test suite()
    {
        return new TestSuite(DefineTextTest.class);
    }

    public void testEqualsPositive()
    {
        DefineText defineText1 = new DefineText(DefineText.stagDefineText2);
        DefineText defineText2 = new DefineText(DefineText.stagDefineText2);
        assertEquals("defineText1 should be equal to defineText2",
                     defineText1, defineText2);
    }

    public void testEqualsNegative()
    {
        DefineText defineText1 = new DefineText(DefineText.stagDefineText);
        DefineText defineText2 = new DefineText(DefineText.stagDefineText2);
        assertFalse("defineText1 should not be equal to defineText2",
                    defineText1.equals(defineText2));
    }

    public void testHashCodePositive()
    {
        DefineText defineText1 = new DefineText(DefineText.stagDefineText2);
        DefineText defineText2 = new DefineText(DefineText.stagDefineText2);        
        assertEquals("the two hash codes should be equal",
                     defineText1.hashCode(), defineText2.hashCode());
    }

    public void testHashCodeNegative()
    {
        DefineText defineText1 = new DefineText(DefineText.stagDefineText);
        DefineText defineText2 = new DefineText(DefineText.stagDefineText2);        
        assertFalse("the two hash codes should not be equal",
                    defineText1.hashCode() == defineText2.hashCode());
    }

    public static void main(String args[])
    {
        DefineTextTest defineTextTest = new DefineTextTest();

        defineTextTest.testEqualsPositive();
        defineTextTest.testEqualsNegative();
        defineTextTest.testHashCodePositive();
        defineTextTest.testHashCodeNegative();
    }
}
