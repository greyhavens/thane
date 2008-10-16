/**

 *  The contents of this file are subject to the Netscape Public License Version 1.1 (the "License");
 *  you may not use this file except in compliance with the License. You may obtain a copy of the
 *  License at http://www.mozilla.org/NPL/ Software distributed under the License is distributed on
 *  an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for 
 *  the specific language governing rights and limitations under the License. The Original Code is 
 *  Mozilla Communicator client code, released March 31, 1998. The Initial Developer of the Original
 *  Code is Netscape Communications Corporation. Portions created by Netscape are 
 *  Copyright (C) 1998-1999 Netscape Communications Corporation. All Rights Reserved.
 *
 *  Contributor(s): Adobe Systems Incorporated.
 *
 *
 *  Author: Gagneet Singh (gasingh@macromedia.com)
 *  Date: 2/4/2005
 *  
 *
 *  SECTION           -- section of testing
 *                    -- for example: "Definitions\const" or "Array"
 *  VERSION           -- either "ECMA3", "Clean AS2", or "AS3"
 *  TITLE             -- sub section
 *  BUGNUMBER         -- leave as ""
 *
 *  Running the test
 *
 *  There are three possible environments that these tests should
 *  run in
 *
 *  spidermonkey  - ECMA 3 tests only, should not need to write any
 *                  more than what we have from mozilla.org
 *
 *                  * use shell.as
 *
 *  Flash MX 2004 - existing Actionscript 2.0 environment.  Only tests
 *                  that are supported by Flash 7 will run here
 *
 *                  * use shellMX2004.as
 *
 *  ASC/ASRT      - All tests written must compile and run here
 *
 *                  * use shellASC.as
 */

var SECTION = "Definitions\const";       				// provide a document reference (ie, ECMA section)
var VERSION = "ActionScript 3.0";  					// Version of JavaScript or ECMA
var TITLE   = "Operations on const variable inside a function";       	// Provide ECMA section title or a description
var BUGNUMBER = "";


startTest();                // leave this alone

/**
 * Calls to AddTestCase here. AddTestCase is a function that is defined
 * in shell.js and takes three arguments:
 * - a string representation of what is being tested
 * - the expected result
 * - the actual result
 *
 * For example, a test might look like this:
 *
 * var helloWorld = "Hello World";
 *
 * AddTestCase(
 * "var helloWorld = 'Hello World'",   // description of the test
 *  "Hello World",                     // expected result
 *  helloWorld );                      // actual result
 *
 */



class myTestConst {

	final const num1 = 1;
	dynamic const num2 = 2;
	static const num3 = 3;

	public static const num6 = 6;
	public final const num7 = 7;
	public dynamic const num8 = 8;
	public final static const num9 = 9;
	dynamic final const num10 = 10;
	dynamic final static const num11 = 11;
	private static const num12 = 12;

	function privatenum2() {
	 	return num2;
	}

	static function privatestaticnum12() {
	 	return num12;
	}



}


// Start the tests.

var myConstObj = new myTestConst();

AddTestCase( "const final property", 1, myConstObj.num3 );
AddTestCase( "const dynamic property", 2, myConstObj.num4 );
AddTestCase( "const static property", 3, myTestConst.num5 );
AddTestCase( "const public static property", 6, myTestConst.num6 );
AddTestCase( "const public final property", 7, myConstObj.num7 );
AddTestCase( "const public dynamic property", 8, myConstObj.num8 );
AddTestCase( "const public final static property", 9, myTestConst.num9 );
AddTestCase( "const dynamic final property", 10, myConstObj.num10 );
AddTestCase( "const dynamic final static property", 11, myTestConst.num11 );
AddTestCase( "const private static property", 12, myTestConst.privatestaticnum12() );


test();       		// Leave this function alone.
			// This function is for executing the test case and then
			// displaying the result on to the console or the LOG file.
