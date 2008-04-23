/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    lastindexof.es
   Description:  lastindexOf(object,value,from=...)
     compares value with every vector element of object in increasing numerical index order, starting at the 
     index from, stopping when an vector lement is equial to value by the === operator, From is rounded toward zero 
     before use.  If from is negative, it is treated as object.length+from, returns vector index from first value or -1 
     if no such element is found.
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */

var SECTION = ""
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( SECTION + " Vector.lastIndexOf()");

/*
var v1=new Vector.<int>();
AddTestCase(	"lastIndexOf empty vector",
		-1,
		v1.lastIndexOf(0));
var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"lastIndexOf object not found",
		-1,
		v1.lastIndexOf(10));
*/
var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"lastIndexOf single match found",
		4,
		v1.lastIndexOf(4));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"lastIndexOf first match found",
		24,
		v1.lastIndexOf(4));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"lastIndexOf first match found setting start parameter",
		14,
		v1.lastIndexOf(4,20));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"lastIndexOf start parameter greater than vector length",
		24,
		v1.lastIndexOf(4,100));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"lastIndexOf start parameter negative",
		14,
		v1.lastIndexOf(4,-10));

test();

