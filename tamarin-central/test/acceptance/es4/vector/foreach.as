/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    foreach.es
   Description:  foreach(object,eacher,thisobj)
     calls checker on every vector element of object in increasing numerical index order,
     collecting all the vector elements for which checker returns a value.
     checker is called with three arguments, the property value, the property index, the object itself.
     the thisobj is used as the this object in the call.
     returns a new vector object containing the elements that were collected in the order they were
     collected.
   Author:       dschaffe@adobe.com 5-Nov-2007
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.forEach()");

function eacher(value,index,obj) {
  result+="("+value+":"+index+")";
}
var bad_eacher="astring";

var v1=new Vector.<int>();
var errormsg="";
try {
  var result=v1.forEach();
} catch (e) {
  errormsg=e.toString();
}
AddTestCase( 	"forEach eacher is undefined",
		"ArgumentError: Error #1063",
		errormsg);

var v1=new Vector.<int>();
for (var i=0;i<3;i++) v1[i]=i;
var errormsg="";
try {
  var result=v1.forEach(bad_eacher);
} catch (e) {
  errormsg=e.toString();
}
AddTestCase( 	"forEach eacher is not a function",
		"TypeError: Error #1034",
		errormsg);

var v1=new Vector.<String>();
for (var i=0;i<3;i++) v1[i]="s"+i;
var result="";
v1.forEach(eacher);
AddTestCase(	"forEach simple vector",
		"(s0:0)(s1:1)(s2:2)",
		result);
test();