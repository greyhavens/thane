/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    unshift.as
   Description:  unshift(object,...items)
     inserts the values in items as new vector elements at the start of object, such
     that their order within the vector elements of object is the same as the order in which
     they appear in items. Existing vector elements in object are shifted upward in the index range
     and the length property of object is updated.
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.unshift()");

var v1=new Vector.<int>();
v1.unshift();
AddTestCase(
		"unshift empty vector with no items still empty",
		"",
		v1.toString());
var v1=new Vector.<int>();
v1.unshift(10);
AddTestCase(
		"unshift empty vector with single item",
		"10",
		v1.toString());

var v1=new Vector.<int>();
v1[0]=10;
v1.unshift(11);
AddTestCase(
		"unshift single element vector with single item",
		"11,10",
		v1.toString());

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
v1.unshift(11);
AddTestCase(
		"unshift small vector with single item",
		"11,0,1,2,3,4,5,6,7,8,9",
		v1.toString());

var v1=new Vector.<int>(3,true);
v1[0]=10; v1[1]=11; v1[2]=12;
var errormsg="";
try {
  v1.unshift(9);
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(
		"unshift single element vector with single item",
		"RangeError: Error #1126",
		errormsg);
test();