/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    15.4.4.3-1.js
   ECMA Section: 15.4.4.3-1 Array.prototype.join()
   Description:  The elements of this object are converted to strings and
   these strings are then concatenated, separated by comma
   characters. The result is the same as if the built-in join
   method were invoiked for this object with no argument.
   Author:       christine@netscape.com, pschwartau@netscape.com
   Date:         07 October 1997
   Modified:     14 July 2002
   Reason:       See http://bugzilla.mozilla.org/show_bug.cgi?id=155285
   Updated:      dschaffe@adobe.com 1 Nov 2007
   ECMA-262 Ed.3  Section 15.4.4.5 Array.prototype.join()
   Step 3: If |separator| is |undefined|, let |separator|
   be the single-character string ","
   *
   */

var SECTION = "15.4.4.3-1";
var VERSION = "ECMA_1";
startTest();

writeHeaderToLog( SECTION + " Array.join()");

var v1=new Vector.<int>();
AddTestCase(	"join empty vector",
		"",
		v1.join());

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"join vector 0-9",
		"0,1,2,3,4,5,6,7,8,9",
		v1.join());

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"join vector with 'and' separator",
		"0 and 1 and 2 and 3 and 4 and 5 and 6 and 7 and 8 and 9",
		v1.join(" and "));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"join vector with '|' separator",
		"0|1|2|3|4|5|6|7|8|9",
		v1.join("|"));

test();
