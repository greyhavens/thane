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

import avmplus.*

if (System.argv.length != 3)
	trace("Usage locDiff fileOne.xml fileTwo.xml output.xml " );
else
{
	var oldFile:XML = XML(File.read(System.argv[0]));
	var newFile:XML = XML(File.read(System.argv[1]));
	var diffableXML:XML = <diffs></diffs>;
	
	//var elements:XMLList = oldFile.descendants("error");
	if (newFile.warnings == undefined)
	{
		for each (var w:XML in newFile..error)
		{
			delete w.description;
			var w2:XMLList = oldFile..error.(@id == w.@id);
			delete w2.description;
			if (w2 == undefined)
			{
				diffableXML.New += <New id={w.@id} label={w.@label}>{String(w)}</New>
			}
			else if (String(w2) != String(w))
			{
				diffableXML.changed += <changed id={w.@id} label={w.@label}>{String(w)}<originalText>{String(w2)}</originalText></changed>
			}
		}
	}
	else
	{
		for each (var w:XML in newFile.warnings.warning)
		{
			delete w.description;
			var w2:XMLList = oldFile.warnings..warning.(@id == w.@id);
			delete w2.description;
			if (w2 == undefined)
			{
				diffableXML.New += <New id={w.@id} label={w.@label}>{String(w)}</New>
			}
			else if (String(w2) != String(w))
			{
				diffableXML.changed += <changed id={w.@id} label={w.@label}>{String(w)}<originalText>{String(w2)}</originalText></changed>
			}
		}
	}
	
	var s:String = "<?xml version='1.0' encoding='utf-8' standalone='no' ?>\n";
	s += diffableXML.toXMLString()
	File.write(System.argv[2], s);
}