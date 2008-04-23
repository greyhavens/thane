/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    indexof.es
   Description:  indexOf(object,value,from=...)
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

writeHeaderToLog( SECTION + " Vector.indexOf()");

var v1=new Vector.<int>();
AddTestCase(	"indexOf empty vector",
		-1,
		v1.indexOf(0));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"indexOf object not found",
		-1,
		v1.indexOf(10));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
AddTestCase(	"indexOf single match found",
		4,
		v1.indexOf(4));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"indexOf first match found",
		4,
		v1.indexOf(4));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"indexOf first match found setting start parameter",
		4,
		v1.indexOf(4,2));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"indexOf start parameter greater than vector length",
		-1,
		v1.indexOf(4,100));

var v1=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
for (var i=0;i<10;i++) v1[i+10]=i;
for (var i=0;i<10;i++) v1[i+20]=i;
AddTestCase(	"indexOf start parameter negative",
		-1,
		v1.indexOf(4,-1));

test();

