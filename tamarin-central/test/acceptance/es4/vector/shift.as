/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    shift.as
   Description:  shift(object)
     removes the element called 0 in object, moves the element at index i+1 to index i,
     and decrements the length property of object by 1.
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.shift()");

var v1=new Vector.<int>();
AddTestCase(
		"shift on empty vector returns undefined",
		0,
		v1.shift());
AddTestCase(
		"shift on empty vector original vector is empty",
		"",
		v1.toString());

var v1=new Vector.<int>();
v1.push(10);
AddTestCase(
		"shift on single element vector returns element[0]",
		10,
		v1.shift());
AddTestCase(
		"shift on single element vector removes first element",
		"",
		v1.toString());

var v1=new Vector.<int>();
for (var i=0;i<5;i++) v1[i]=10+i;
AddTestCase(
		"shift on vector returns element[0]",
		10,
		v1.shift());
AddTestCase(
		"shift on vector removes first element",
		"11,12,13,14",
		v1.toString());

var v1=new Vector.<int>(10);
AddTestCase(
		"shift on initialized vector returns element[0]",
		0,
		v1.shift());
AddTestCase(
		"shift on initialized vector removes first element",
		"0,0,0,0,0,0,0,0,0",
		v1.toString());

var v1=new Vector.<int>(10,true);
for (var i=0;i<10;i++) v1[i]=10+i;
var errormsg="";
try {
  v1.shift();
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(
		"shift on fixed vector throws exception",
		"RangeError: Error #1126",
		errormsg);
test();AddTestCase(
		"shift on fixed vector does shift",
		"10,11,12,13,14,15,16,17,18,19",
		v1.toString());
test();