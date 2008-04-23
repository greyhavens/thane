/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    typechecking.as
   Description:  
   verify runtime error when set or add mismatched types.
   Author:       dschaffe@adobe.com 25-Mar-2008
   *
   */
var SECTION="";
var VERSION = "ECMA_4";

startTest();

writeHeaderToLog( " Vector.typechecking");

// untyped
var vs1=new Vector.<String>();
// typed
var vs2:Vector.<String>=new Vector.<String>();
var v1:Vector.<Object>;

var err1="no error";
try {
  v1=vs2;
} catch (e) {
  err1=e.toString();
}

AddTestCase(
		"assign vector variable to mismatched builtin type, throws runtime error",
		"TypeError: Error #1034",
		err1);

class A { }
class B extends A { }
class C { }

var v2:Vector.<A>=new Vector.<A>();
var v3:Vector.<B>=new Vector.<B>();
try {
  v2=v3;
} catch (e) {
  err3=e.toString();
}
AddTestCase(
		"assign vector variable to mismatched types, throws runtime error",
		"TypeError: Error #1034",
		err3);


AddTestCase(
		"push inherited type into Vector is allowed",
		2,
		v2.push(new A(),new B()));

var err4="no error";
try {
  v2.push(new A(),new C());
} catch (e) {
  err4=e.toString();
}
AddTestCase(
		"push wrong type into Vector, throws runtime error",
		"TypeError: Error #1034",
		err4);

var v4:Vector.<A>=new Vector.<A>();

AddTestCase(
		"unshift inherited types is allowed",
		2,
		v4.unshift(new A(),new B()));

AddTestCase(
		"unshift inherited types is allowed, verify toString",
		"[object A],[object B]",
		v4.toString());

var err5="no error";
var v5=new Vector.<A>();
try {
 v5.unshift(new A(),new B(),new C());
} catch (e) {
 err5=e.toString();
}
AddTestCase(
		"unshift wrong type into Vector, throws runtime error",
		"TypeError: Error #1034",
		err5);

var v6:Vector.<A>=new Vector.<A>();
v6.push(new A(),new A());
var v6a:Vector.<B>=new Vector.<B>();
v6a.push(new B(),new B());
AddTestCase(
		"concat inherited types returns correct vector",
		"[object A],[object A],[object B],[object B]",
		v6.concat(v6a).toString());

var err7="no error";
var v7:Vector.<C>=new Vector.<C>();
v7.push(new C());
try {
  v6.concat(v7);
} catch (e) {
  err7=e.toString();
}
AddTestCase(
		"vector concat throws runtime error",
		"TypeError: Error #1034",
		err7);

var v8:Vector.<A>=new Vector.<A>();
var err8="no error";
try {
  v8[0]=new C();
} catch(e) {
  err8=e.toString();
}
AddTestCase(
		"vector assignment on incorrect type throws runtime error",
		"TypeError: Error #1034",
		err8);

test();