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
 *  Author:             christine@netscape.com
 *  Date:               11 August 1998
 *
 *  Modified date 1/18/2005 by Sushant Dutta(sdutta@macromedia.com)
 *  Removed the eval function.
 *
 */
 
    var SECTION = "";
    var VERSION = "ECMA_2";
    var TITLE   = "Keywords";

    startTest();

    var result = "failed";

    try {
		super;
	} 
	catch (x) {
		if (x instanceof SyntaxError)
			result = x.name;
	}

    AddTestCase(
        "using the expression \"super\" shouldn't cause js to crash",
        "SyntaxError",
        result );

    test();
