
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
 *  Author: Muralikrishna Nidugala (mnidugal@macromedia.com)
 *  Date: 03/22/2005
 *
 *  Modifications: (Name   :Date)
 *
 *  SECTION           -- section of testing
 *                    -- for example: "Directives" or "Array"
 *  VERSION           -- either "ECMA3", "Clean AS2", or "AS2.1"
 *  TITLE             -- sub section
 *  BUGNUMBER         -- leave as ""
 *
 *  Running the test
 *
 *  ASC/ASRT      - All tests written must compile and run here
 *
 *                  * use shellASC.as
 */
 /*
Access the private method defined in varsDef.abc
*/

var SECTION = " ";
var VERSION = "AS3";
var TITLE   = "import a class defined in .abc file";

startTest();
writeHeaderToLog( SECTION + " "+ TITLE);

class accSpecs extends privClass {

}

obj = new accSpecs();

AddTestCase( "use namespace 'Baseball' ", "Inside testClass", obj.finFunc1());
AddTestCase( "use namespace 'Baseball' ", "Inside testClass", obj.privFunc());
AddTestCase( "use namespace 'Baseball' ", "Inside testClass", obj.statClassVar1);


test();


