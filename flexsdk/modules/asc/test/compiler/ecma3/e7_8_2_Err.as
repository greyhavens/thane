
/*  The contents of this file are subject to the Netscape Public License Version 1.1 (the "License");
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
 */
/**
    File Name:          7.8.2.js
    ECMA Section:       7.8.2 Examples of Automatic Semicolon Insertion
    Description:        compare some specific examples of the automatic
                        insertion rules in the EMCA specification.
    Author:             christine@netscape.com
    Date:               15 september 1997
*/

    var SECTION="7.8.2";
    var VERSION="ECMA_1"
    startTest();
    writeHeaderToLog(SECTION+" "+"Examples of Semicolon Insertion");

    testcases = getTestCases();
    test();

function getTestCases() {
    var array = new Array();
    var item = 0;
    var a:Number=0;
    var b:Number=10;
    var c:Number=20;
    var d:Number=30;
    
   
   // array[item++] = new TestCase( "7.8.2",  "{ 1 2 } 3",         "error",   eval("{1 2 }    //3")     );
    
   {1 2 } 3;
   array[item++] = new TestCase( "7.8.2",  "Semicolon not inserted automatically for { 1 2 } 3 and it is not a valid sentence in ECMAScript","error",   "{1 2} 3");
    
  

if(a>b)
else c=d;
   
   array[item++] = new TestCase( "7.8.2",  "Semicolon not inserted automatically in the if(a>b)\n else c=d and it is not a valid sentence in ECMAScript","error","if(a>b)\nelse c=d");
    
   

thisError="no error";
try{
    if(b>a){
    throw 
"b is greater than a";
    }
   }catch(e){
         thisError=e.toString();
     }finally{
      }

    array[item++] = new TestCase( "7.8.2",  "Semicolon  inserted automatically after throw so it is throwing compiler error","error","throw\nb is greater than a");
thisError="no error";
         


    return ( array );
}

