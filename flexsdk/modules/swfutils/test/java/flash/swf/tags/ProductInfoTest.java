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

package flash.swf.tags;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;
import flash.swf.Movie;
import flash.swf.MovieEncoder;
import flash.swf.TagDecoder;
import flash.swf.TagEncoder;
import flash.swf.types.Rect;

public class ProductInfoTest extends TestCase {


    public ProductInfoTest() {
    }

    protected void setUp() throws Exception {

    }

    public static Test suite() {
        return new TestSuite(ProductInfoTest.class);
    }

    public void testCompileDate() throws IOException {

        long time = System.currentTimeMillis();

        // encode the movie with the product info (compilation time)
        TagEncoder te = new TagEncoder();
        MovieEncoder me = new MovieEncoder(te);

        Movie movie = new Movie();
        movie.size = new Rect(100, 100);
        movie.frames = new ArrayList();
        movie.productInfo = new ProductInfo(time);
        me.export(movie);

        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        te.writeTo(bos);

        // decode the movie and save the compilation time in the special handler
        ProductInfoTagHandler decodeHandler = new ProductInfoTagHandler();
        TagDecoder decoder = new TagDecoder(new ByteArrayInputStream(bos.toByteArray()));
        decoder.parse(decodeHandler);

        // verify that the compilation time after encode/decode is the same as when you set it
        assertEquals("time should be equivalent", time, decodeHandler.getProductInfo().getCompileDate());
    }
}