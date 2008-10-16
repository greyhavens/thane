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

import flash.swf.TagHandler;

public class ProductInfoTagHandler extends TagHandler
{
    private ProductInfo pi;

    public void productInfo(ProductInfo tag)
    {
        this.pi = tag;
    }

    public ProductInfo getProductInfo()
    {
        return pi;
    }
}
