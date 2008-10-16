/* The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, released March
 * 31, 1998.
 *
 * The Initial Developer of the Original Code is Netscape Communications
 * Corporation. Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): Adobe Systems Incorporated
 *
 */
/**
	Filename:     TryAndCatchWithSttsInbetweenErr.as
	Description:  'This tests the try and catch Statement with statements in between try                         and catch'

	Author:       Subha Subramanian
	Date:         09/14/2005
*/

	var SECTION = '12.13';
	var VERSION = 'no version';
    startTest();
	var TITLE = 'Statement:Try and Catch with statements in between them';

	writeHeaderToLog('Executing script: TryAndCatchWithSttsInbetweenErr.as');
	writeHeaderToLog( SECTION + " "+ TITLE);

	var count = 0;
	var testcases = new Array();

	var t;
	var z = 1;


	try {
		 if(z == 1)
			throw "Error 1"
		 else if(z == 2)
			throw "Error 2"
	    }

	catch(er) {
	    if(er == "Error 1")
		t = "Error 1"
	    if(er == "Error 2")
		t = "Error 2"
                  }
	var statementBetweenTryAndCatch;
        catch(er1){
//                  print("Should fail");
                  }

	testcases[count++] = new TestCase( SECTION, "throw t", "Error 1", t);


	function test()
	{
	   for ( tc=0; tc < testcases.length; tc++ ) {
	        testcases[tc].passed = writeTestCaseResult(
	        testcases[tc].expect,
	        testcases[tc].actual,
	        testcases[tc].description +" = "+
	        testcases[tc].actual );
	        testcases[tc].reason += ( testcases[tc].passed ) ? "" : "wrong value ";
	   }
	   stopTest();
	   return ( testcases );
	}

	test();

