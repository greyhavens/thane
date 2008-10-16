/**
 *  Class ExtOverLoadDefaultErr
 *
 *  extends DefaultClass
 *
 *  This class overrides methods of the parent class
 *  with default methods in the sub class
 *
 *	Modified: 03/23/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	class DefaultClass {

	     var array;
	     public var pubArray;
	     private var privArray;
	     static var statArray;
	     public static var pubStatArray;
	     private static var privStatArray;


	     function getBoolean() : Boolean { return true; }

	     public function setPubBoolean( b:Boolean ) { return b; }

	     private function setPrivBoolean( b:Boolean ) { return b; }

	     static function setStatBoolean( b:Boolean ) { return b; }

	     public static function setPubStatBoolean( b:Boolean ) { return b; }

	     private static function setPrivStatBoolean( b:Boolean ) { return b; }
	 }

	class ExtOverLoadDefaultErr extends DefaultClass {

	    var array;
	    var pubArray;
	    var privArray;
	    var statArray;
	    var pubStatArray;
	    var privStatArray;


	    // *******************************
	    // override a default method
	    // with a default method
	    // *******************************

	    function getBoolean() : Boolean { return true; }

	    // *******************************
	    // override a public method
	    // with a default method
	    // *******************************

	    function setPubBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a private method
	    // with a default method
	    // *******************************

	    function setPrivBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a static method
	    // with a default method
	    // *******************************

	    function setStatBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a public static method
	    // with a default method
	    // *******************************

	    function setPubStatBoolean( b:Boolean ) { return b; }

	    // ********************************
	    // override a private static method
	    // with a default method
	    // ********************************

	    function setPrivStatBoolean( b:Boolean ) { return b; }
	}
}
