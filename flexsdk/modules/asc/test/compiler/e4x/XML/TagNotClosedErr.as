
/**
 *  This template is a modified version of the test case
 *  templates taken from mozilla.org.  This template or
 *  any test cases written from it are not for external
 *  use or distribution.
 *  
 *  Author: Vera Fleischer (vfleischer@macromedia.com)
 *  Date: 03/07/2005
 *
 *  Modifications: (Name   :Date)
 *
 *  SECTION           -- section of testing
 *
 *  Running the test
 *
 *
 *  ASC/AVMPlus   - All tests written must compile and run here
 *
 *                  * use shellASC.as
 *
 */

var SECTION = "XML tag not closed";       // provide a document reference (ie, Actionscript section)
var VERSION = "AS 3.0";        // Version of ECMAScript or ActionScript 
var TITLE   = "Compiler error: XML tag not closed";       // Provide ECMA section title or a description
var BUGNUMBER = "";

START(TITLE);          


xml = 
<root>
<blah a="attr"
hi
</blah>
</root>;


test();
