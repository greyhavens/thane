/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    splice.as
   Description:  splice(object,start,deletCount,...items)
     splice replaces the deleteCount vector elements of object starting at vector index start with values
     from the items, the methods returns a new vector object containing the vector elements that were removed
     from objects, in order.
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

/*
writeHeaderToLog( " Vector.splice()");
var v1=new Vector.<String>();
AddTestCase(
		"splice empty vector with nothing",
		"",
		v1.splice().toString());

var v1=new Vector.<String>();
v1.splice(0,0);
AddTestCase(
		"splice empty vector with nothing set start",
		"",
		v1.toString());
var v1=new Vector.<String>();
v1[0]="one";v1[1]="two";v1[2]="three";
var splice=v1.splice(2,0,"three","four")
AddTestCase(
		"splice small vector no delete",
		"one,two,three,four",
		v1.toString());
*/
var v1=new Vector.<String>();
v1.push("one");v1.push("delete");v1.push("delete");v1.push("two");v1.push("three");
var splice=v1.splice(1,2)
AddTestCase(
		"splice small vector delete 2 items, no add",
		"one,two,three",
		v1.toString());
var v1=new Vector.<String>();
v1.push("one");v1.push("delete");v1.push("delete");v1.push("four");v1.push("five");v1.push("six");
var splice=v1.splice(1,2,"two","three")
AddTestCase(
		"splice small vector delete 2 items, add 2 items",
		"one,two,three,four,five,six",
		v1.toString());

var v1=new Vector.<String>();
v1.push("one");v1.push("delete1");v1.push("delete2");v1.push("four");v1.push("five");v1.push("six");
var splice=v1.splice(-5,2,"two","three")
AddTestCase(
		"splice small vector start is negative",
		"one,two,three,four,five,six",
		v1.toString());
/*
var v1=new Vector.<String>();
v1.push("one");v1.push("delete1");v1.push("delete2");v1.push("four");v1.push("five");v1.push("six");
var splice=v1.splice(1,-2,"two","three")
AddTestCase(
		"splice small vector deletecount is negative",
		"one,two,three,delete1,delete2,four,five,six",
		v1.toString());
*/
test();