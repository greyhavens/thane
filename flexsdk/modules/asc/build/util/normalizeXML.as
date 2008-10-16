////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

/** normalizeXML.as
/*   This script merely reads in an XML format file and writes it out again using the default E4X xml formating.  Run this
*     script on a source file to establish a baseline standard format before using the generateLocDiffs/integrateLocDiffs
*     scripts (else all the formating changes between the hand rolled file and the E4X formatted file will make diff comparisions difficult(
*
*      chris nuuja  6/25/06
*/

import avmplus.*
		
if (System.argv.length != 2)
{
	trace("Usage avmplus normalizeXML.abc -- originalFile normalizedFileName " );
}
else
{
	var origFile:String = System.argv[0];
	var outputFile:String = System.argv[1];
	
	var normFile:XML = XML(File.read(origFile));
	
	// output the result.
	var s:String = "<?xml version='1.0' encoding='utf-8' standalone='no' ?>\n";
	s += normFile.toXMLString()
	File.write(outputFile, s);
}