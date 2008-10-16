/**
 *  Class ExtOverLoadPrivateErr
 *
 *  extends DefaultClass
 *
 *  This class overrides methods of the parent class with
 *  private methods
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


	class ExtOverLoadPrivateErr extends DefaultClass {

	    private var array;
	    private var pubArray;
	    private var privArray;
	    private var statArray;
	    private var pubStatArray;
	    private var privStatArray;


	    // *******************************
	    // override a default method
	    // with a private method
	    // *******************************

	    private function getBoolean() : Boolean { return true; }

	    // *******************************
	    // override a public method
	    // with a private method
	    // *******************************

	    private function setPubBoolean( b:Boolean ) { return true; }

	    // *******************************
	    // override a private method
	    // with a private method
	    // *******************************

	    private function setPrivBoolean( b:Boolean ) { return true; }

	    // *******************************
	    // override a static method
	    // with a private method
	    // *******************************

	    private function setStatBoolean( b:Boolean ) { return true; }

	    // *******************************
	    // override a public static method
	    // with a private method
	    // *******************************

	    private function setPubStatBoolean( b:Boolean ) { return true; }

	    // ********************************
	    // override a private static method
	    // with a private method
	    // ********************************

	    private function setPrivStatBoolean( b:Boolean ) { return true; }
	}
}
