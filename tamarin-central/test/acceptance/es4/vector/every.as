/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    every.es
   Description:  every(object,checker,thisObj=)
     calls checker on every Vector element of object in increasing numerical index order, stopping
     as soon as any call returns false.
     checker is called with three arguments: the property value, the property index
     and the object itself.  The thisobj is used as the this object in the call.
     returns true if all the calls to checker returned true values, otherwise it returns false.
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.every()");

function checker1(value,index,obj):Boolean {
  msg+="checker1("+value+","+index+",["+obj+"])";
  if (value==0)
    return false;
  return true;
}
function checker3(value,index,obj):Boolean {
  msg+=this.message;
  return true;
}

var msg="";
var v1=new Vector.<int>();
AddTestCase(	"every empty array",
		true,
		v1.every(checker1));

var msg="";
var v1=new Vector.<int>();
for (var i=0;i<3;i++) v1.push(i+1);
AddTestCase(	"every small array returns true",
		true,
		v1.every(checker1));

AddTestCase(	"every small array check function",
		"checker1(1,0,[1,2,3])checker1(2,1,[1,2,3])checker1(3,2,[1,2,3])",
		msg);

var msg="";
var v1=new Vector.<int>();
for (var i=0;i<3;i++) v1.push(2-i);
AddTestCase(    "every small array returns false on 0",
                false,
                v1.every(checker1));


var msg="";
var thisobj=new Object();
thisobj.message="object";
var v1=new Vector.<int>(5);
v1.every(checker3,thisobj);
AddTestCase(	"every small array with a specified this object",
		"objectobjectobjectobjectobject",
		msg);
/*
var msg="";
var v1=new Vector.<int>();
for (var i=0;i<3;i++) v1.push(i+1);
AddTestCase(	"static every small array returns true",
		true,
		Vector.<int>.every(v1,checker1));

AddTestCase(	"every small array check function",
		"checker1(1,0,[1,2,3])checker1(2,1,[1,2,3])checker1(3,2,[1,2,3])",
		msg);
*/
test();