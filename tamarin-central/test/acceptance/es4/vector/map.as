/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    map.as
   Description:  map(object,mapper,thisobj)
     calls mapper on each vector element of object in increasing numerical index order, collecting
     the return values from mapper in a new vector object.
     mapper is called with three arguments: the property value, the property index, and object itself.
     The thisobj is used as the this object in the call.
     returns a new vector object where the vector element at index i is the value returned from the call
     to mapper on object[i].
   Author:       dschaffe@adobe.com 5-Nov-2007
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.map()");

function mapper1(value,index,obj) {
   return "("+value+":"+index+")";
}
var mapper2="a string";
function mapper3(value,index,obj) {
   return "("+this.message+")";
}
function mapper4(value,index,obj) {
   return value*value;
}

var v1=new Vector.<int>();
v1.push(1);
var errormsg="";
try {
  var result=v1.map();
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(
		"map mapper is undefined",
		"ArgumentError: Error #1063",
		errormsg);

var v1=new Vector.<int>();
v1.push(1);
var errormsg="";
try {
  var result=v1.map(mapper2);
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(
		"map mapper is not a function",
		"TypeError: Error #1034",
		errormsg);

var v1=new Vector.<int>();
AddTestCase(
		"map empty vector",
		"",
		v1.map(mapper1).toString());

var v1=new Vector.<String>();
v1[0]='a';
v1[1]='b';
v1[2]='c';
AddTestCase(
		"map small vector",
		"(a:0),(b:1),(c:2)",
		v1.map(mapper1).toString());

var v1=new Vector.<String>(3,true);
v1[0]='a';
v1[1]='b';
v1[2]='c';
AddTestCase(
		"map fixed size small vector",
		"(a:0),(b:1),(c:2)",
		v1.map(mapper1).toString());

testobj=new Object();
testobj.message="testobj";
var v1=new Vector.<String>();
v1.push('a');v1.push('b');
AddTestCase(   "map vector passing new object",
               "(testobj),(testobj)",
               v1.map(mapper3,testobj).toString());

var v1=new Vector.<int>();
v1[0]=1;
v1[1]=2;
v1[2]=3;
v1[3]=4;
AddTestCase(   "map vector of int",
               "1,4,9,16",
               v1.map(mapper4).toString());

test();