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
 *  Author: Michael Tilburg (mtilburg@macromedia.com)
 *  Date: 5/6/2004
 *
 *  Modified: 03/22/2005 (gasingh@macromedia.com) 
 *  Added the package Errors to encapsulate the test case code.
 *

 *  SECTION           -- section of testing
 *                    -- for example: "Directives" or "Array"
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

//var SECTION = "Definitions";       // provide a document reference (ie, ECMA section)
//var VERSION = "Clean AS2";  // Version of JavaScript or ECMA
//var TITLE   = "Variables";       // Provide ECMA section title or a description
//var BUGNUMBER = "";

//startTest();                // leave this alone

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

// ******************************************
// Error case... this file should not
// compile!
// ******************************************

package Errors {

    	// <empty> Identifier, Identifier : TypeExpression = AssignmentExpression
    	idAAssign = false, idBAssign:Boolean = true; 
    	idAAssignB, idBAssignB:Boolean = true; 
}

//test();       // leave this alone.  this executes the test cases and
              // displays results.