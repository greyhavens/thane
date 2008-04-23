/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    concat.es
   Description:  The static concat method collects the vector elements from object followed by the vector
    elements from the additional items, in order, into a new vector object.  All the items must be objects.
    returns a new vector object
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */

var SECTION = " ";
var VERSION = "AS3";
startTest();

writeHeaderToLog( SECTION + " Vector.concat()");

var v1=new Vector.<uint>();
v1[0]=0; v1[1]=1; v1[2]=2;
var v2=new Vector.<uint>();
v2[0]=3;v2[1]=4;v2[2]=5;
var v3=v1.concat(v2)
AddTestCase(    "concat uint vector, original vector is unchanged",
		"0,1,2",
		v1.toString());
AddTestCase(	"concat uint vector, new vector concat worked",
		"0,1,2,3,4,5",
		v3.toString());

/*
var v1=new Vector.<int>();
var v2=v1.concat(1,2,3);
AddTestCase(    "concat single elements as parameters",
                "1,2,3",
                v2.toString());
*/

var v1=new Vector.<String>();
v1[0]="zero"; v1[1]="one"; v1[2]="two";
var v2=new Vector.<int>();
v2[0]=0; v2[1]=1; v2[2]=2;
var errormsg;
try {
  var v3=v1.concat(v2);
} catch (e) {
  errormsg=e.toString();
}
AddTestCase(    "concat two differently typed vectors",
                "TypeError: Error #1034",
                errormsg);

/*
var v1=new Vector.<int>();
v1[0]=0; v1[1]=1; v1[2]=2;
var v2=new Vector.<int>();
v2[0]=3; v2[1]=4; v2[2]=5;
var v3=Vector.<int>.concat(v1,v2);
AddTestCase(    "static Vector.<int>.concat two simple vectors",
                "0,1,2,3,4,5",
                v3.toString());
*/
test();

