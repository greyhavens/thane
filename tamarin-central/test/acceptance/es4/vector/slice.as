/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    slice.as
   Description:  slice(object,start=...,end=...)
     extracts the subrange of vector elements from object between start (inclusive)
     and end (exclusive) into a new vector.
     if start is negative, it is treated as object.length+start. If end is negative, it is treated as
     object.length+end.  In either case the values of start and end are bounded between 0 and
     object.length
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.slice()");
/*
var v1=new Vector.<String>();
AddTestCase(
		"slice no args on empty vector",
		"",
		v1.slice().toString());
var v1=new Vector.<String>();
AddTestCase(
		"slice args out of bounds on empty vector",
		"",
		v1.slice(5,6).toString());
var v1=new Vector.<String>();
v1.push("one");v1.push("two");v1.push("three");
AddTestCase(
		"slice args out of bounds on small vector",
		"",
		v1.slice(5,6).toString());

var v1=new Vector.<String>();
v1.push("one");v1.push("two");v1.push("three");
AddTestCase(
		"slice invalid args end less than start on small vector",
		"",
		v1.slice(2,1).toString());
*/

var v1=new Vector.<String>();
v1.push("one");v1.push("two");v1.push("three");v1.push("four");v1.push("five");v1.push("six");
AddTestCase(
		"slice on small vector",
		"two,three",
		v1.slice(1,3).toString());
/*
var v1=new Vector.<String>();
v1.push("one");v1.push("two");v1.push("three");v1.push("four");v1.push("five");
AddTestCase(
		"slice start negative on small vector",
		"two,three",
		v1.slice(-4,3).toString());

var v1=new Vector.<String>();
v1.push("one");v1.push("two");v1.push("three");v1.push("four");v1.push("five");
AddTestCase(
		"slice end negative on small vector",
		"two,three",
		v1.slice(1,-2).toString());
*/
test();