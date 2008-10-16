////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.fonts;

import org.apache.batik.svggen.font.Font;
import org.apache.batik.svggen.font.table.Os2Table;

/**
 * Returns information on the fsType of the font.  More information on this can be found here:
 * http://partners.adobe.com/public/developer/en/acrobat/sdk/FontPolicies.pdf
 * http://www.microsoft.com/typography/tt/ttf_spec/ttch02.doc
 *
 * And if we start using OpenType, we should follow this:
 * http://www.microsoft.com/typography/otspec/os2.htm
 *
 * @author Brian Deitte
 */
public class FSType
{
	public int fsType;
	public String description;
	public boolean installable, editable, previewAndPrint, noEmbedding, usableByFlex;

	public FSType(int fsType, String description, boolean installable, boolean editable, boolean previewAndPrint, boolean noEmbedding)
	{
		this.fsType = fsType;
		this.description = description;
		this.installable = installable;
		this.editable = editable;
		this.previewAndPrint = previewAndPrint;
		this.noEmbedding = noEmbedding;
		this.usableByFlex = true; //installable || editable;
	}

	public static FSType getFSType(Font font)
	{
		Os2Table table = font.getOS2Table();
		return getFSType(table.getLicenseType());
	}

	public static FSType getFSType(int lt)
	{
		String description;
        boolean installable = false, editable = false, previewAndPrint = false, noEmbedding = false;
		boolean noEmbeddingBit = (lt & 0x0002) == 0x0002;
		boolean previewAndPrintBit = (lt & 0x0004) == 0x0004;
		boolean editableBit = (lt & 0x0008) == 0x0008;
		// the most permissible bit wins
		if (editableBit)
		{
			editable = true;
			description = "Editable embedding";
		}
		else if (previewAndPrintBit)
		{
			previewAndPrint = true;
			description = "Preview and Print embedding";
		}
		else if (noEmbeddingBit)
		{
			noEmbedding = true;
			description = "No embedding allowed";
		}
		else
		{
			installable = true;
			description = "Installable embedding";
		}

		return new FSType(lt, description, installable, editable, previewAndPrint, noEmbedding);
	}
}
