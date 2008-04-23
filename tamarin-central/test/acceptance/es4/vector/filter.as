/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    filter.es
   Description:  filter(object,checker,thisobj)
     calls checker on every vector element of object in increasing numerical index order,
     collecting all the vector elements for which checker returns a true value.
     checker is called with three arguments: the property value, the property index, and object
     itself. The thisobj is used as the this object in the call.
     returns a new vector object containing the elements that were collected in the order
     they were collected.
   Author:       dschaffe@adobe.com 5-Nov-2007
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.filter()");

function EvenChecker(value,index,obj) {
  if (value%2==0)
    return true;
  return false;
}
var invalidchecker="a string";
function ThisChecker(value,index,obj):Boolean {
  msg+=this.message;
  return true;
}

var v1=new Vector.<int>();
var errormsg="";
try {
  var result=v1.filter();
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(	"filter checker is undefined",
		"ArgumentError: Error #1063",
		errormsg);

var v1:Vector.<int>=new Vector.<int>(10);
for (var i=0;i<10;i++) v1[i]=i;
var errormsg="";
try {
  var result=v1.filter(invalidchecker);
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(	"filter checker is not a function",
		"TypeError: Error #1034",
		errormsg);

var v1:Vector.<int>=new Vector.<int>();
var result=v1.filter(EvenChecker);
AddTestCase(	"filter empty vector",
		"",
		result.toString());

var v1:Vector.<int>=new Vector.<int>();
for (var i=0;i<10;i++) v1[i]=i;
var result=v1.filter(EvenChecker);
AddTestCase(	"filter small vector",
		"0,2,4,6,8",
		result.toString());

var v1:Vector.<int>=new Vector.<int>();
for (var i=0;i<3;i++) v1[i]=i;
var myobject=new Object();
myobject.message="message";
var msg="";
var result=v1.filter(ThisChecker,myobject);
AddTestCase(	"filter use thisobj",
		"messagemessagemessage",
		msg);

test();