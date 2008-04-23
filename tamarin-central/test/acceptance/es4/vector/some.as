/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    some.as
   Description:  some(object,checker,thisobj=)
     calls checker on every vector element in object in increasing numerical index order,
     stopping as soon as checker returns a true value.
     checker is called with three arguments: the property value, the property index, and the object
     itself.  The thisobj is used as the this object in the call.
     returns true when checker returns a true value, otherwise returns false if all the calls to checker
     return false values.
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog(" Vector.some()");
var msg;
function checker1(value,index,obj) {
  msg+="(value="+value+",index="+index+",object=["+obj+"])";
  if (value=='t')
    return true;
  return false;
}

var v1=new Vector.<int>();
var errormsg="";
try {
  result=v1.some();
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(
		"some no checker",
		"ArgumentError: Error #1063",
		errormsg);


var checker2="a string";
var v1=new Vector.<int>();
v1.push(1);
var errormsg="";
try {
  result=v1.some(checker2);
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(
		"some checker not a function",
		"TypeError: Error #1034",
		errormsg);

var msg="";
var v1=new Vector.<int>();
var result=v1.some(checker1);
AddTestCase(
                "some empty vector result",
                false,
                result);
AddTestCase(
                "some empty vector message empty",
                "",
                msg);
var msg="";
var v1=new Vector.<String>();
v1[0]='a';v1[1]='b';v1[2]='c';
var result=v1.some(checker1);
AddTestCase(
                "some small vector result",
                false,
                result);
AddTestCase(
                "some small vector message",
                "(value=a,index=0,object=[a,b,c])(value=b,index=1,object=[a,b,c])(value=c,index=2,object=[a,b,c])",
                msg);

var msg="";
var v1=new Vector.<String>();
v1[0]='a';v1[1]='b';v1[2]='t';v1[3]='c';v1[4]='d';
var result=v1.some(checker1);
AddTestCase(
                "some small vector result with a true",
                true,
                result);
AddTestCase(
                "some small vector message with a true",
                "(value=a,index=0,object=[a,b,t,c,d])(value=b,index=1,object=[a,b,t,c,d])(value=t,index=2,object=[a,b,t,c,d])",
                msg);
test();