/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    push.es
   Description:  push(object,...items)
     push method adds elements to the end of the vector and increases the length of the vector
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " Vector.push()");

var v1=new Vector.<int>();
v1.push();
AddTestCase(
		"push empty vector",
		"",
		v1.toString());

var v1=new Vector.<int>();
v1.push(1)
AddTestCase(
                "push single item to item vector",
                "1",
                v1.toString());

var v1=new Vector.<int>();
for (var i=0;i<3;i++) v1[i]=i;
v1.push()
AddTestCase(
                "push nothing to small vector",
                "0,1,2",
                v1.toString());

/*
var v1=new Vector.<int>();
for (var i=0;i<3;i++) v1[i]=i;
v1.push(4,5,6);
AddTestCase(
                "push several elements to small vector",
                "0,1,2,3,4,5,6",
                v1.toString());
*/
test();