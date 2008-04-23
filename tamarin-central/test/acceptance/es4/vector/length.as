/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:          length.as
   ECMA Section:       Array.length
   Description:
   15.4.5.2 length
   The length property of this Array object is always numerically greater
   than the name of every property whose name is an array index.

   The length property has the attributes { DontEnum, DontDelete }.
   Author:             christine@netscape.com 12 Nov 1997
   Updated:            dschaffe@adobe.com 2 Nov 2007
*/

var SECTION = "15.4.5.2-1";
var VERSION = "ECMA_1";
startTest();
var TITLE   = "Vector.length";

writeHeaderToLog( SECTION + " "+ TITLE);


var v1=new Vector.<int>();
AddTestCase(	"length of empty vector",
		0,
		v1.length);

var v1=new Vector.<int>(1999);
AddTestCase(	"length of initialized vector of size 1999",
		1999,
		v1.length);

var v1=new Vector.<int>(1999);
v1.push(1);
AddTestCase(	"length of initialized vector",
		2000,
		v1.length);

var v1=new Vector.<int>();
for (var i=0;i<100;i++) v1[i]=i;
AddTestCase(	"length of empty vector with 100 elements set",
		100,
		v1.length);

var v1=new Vector.<int>(100,true);
AddTestCase(	"length of fixed size vector",
		100,
		v1.length);

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
v1.length=5;
AddTestCase(	"reduce length of vector removes elements reduces length",
		5,
		v1.length);
AddTestCase(	"reduce length of vector removes elements past new length",
		"0,1,2,3,4",
		v1.toString());
v1.length=10;
AddTestCase(	"increase length of array with removed elements increases length",
		10,
		v1.length);
AddTestCase(	"increase length of array with removed elements adds empty elements",
		"0,1,2,3,4,0,0,0,0,0",
		v1.toString());

test();

