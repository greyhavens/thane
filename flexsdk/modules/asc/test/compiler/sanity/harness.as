include "shell.as"
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
 * Contributor(s): 
 * 
 */
/**
    File Name:      15.6.1.js
    ECMA Section:   15.6.1 The Boolean Function
                    15.6.1.1 Boolean( value )
                    15.6.1.2 Boolean ()
    Description:    Boolean( value ) should return a Boolean value
                    not a Boolean object) computed by
                    Boolean.toBooleanValue( value)

                    15.6.1.2 Boolean() returns false

    Author:         christine@netscape.com
    Date:           27 jun 1997


    Data File Fields:
        VALUE       Argument passed to the Boolean function
        TYPE        typeof VALUE (not used, but helpful in understanding
                    the data file)
        E_RETURN    Expected return value of Boolean( VALUE )
*/
    var SECTION = "SMOKE";
    var VERSION = "ECMA_1";
    startTest();
    var TITLE   = "Test harness for mozilla tests";

    writeHeaderToLog( SECTION + " "+ TITLE);

    var testcases = getTestCases();

    test();

function getTestCases() {
    var array = new Array();
    var item = 0;

    array[item++] = new TestCase( SECTION,   "test()",         true,   true );
    return ( array );
}
function test() {
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
