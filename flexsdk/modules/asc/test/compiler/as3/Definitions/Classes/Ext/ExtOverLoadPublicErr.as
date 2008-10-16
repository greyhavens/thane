/**
 *  Class ExtOverLoadPublicErr
 *
 *  extends DefaultClass
 *
 *  This class overrides methods of the parent class with
 *  public methods
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


	class ExtOverLoadPublicErr extends DefaultClass {

	    public var array;
	    public var pubArray;
	    public var privArray;
	    public var statArray;
	    public var pubStatArray;
	    public var privStatArray;


	    // *******************************
	    // override a default method
	    // with a public method
	    // *******************************

	    public function getBoolean() : Boolean { return true; }

	    // *******************************
	    // override a public method
	    // with a public method
	    // *******************************

	    public function setPubBoolean( b:Boolean ) { return true; }

	    // *******************************
	    // override a private method
	    // with a public method
	    // *******************************

	    public function setPrivBoolean( b:Boolean ) { return true; }

	    // *******************************
	    // override a static method
	    // with a public method
	    // *******************************

	    public function setStatBoolean( b:Boolean ) { return true; }

	    // *******************************
	    // override a public static method
	    // with a public method
	    // *******************************

	    public function setPubStatBoolean( b:Boolean ) { return true; }

	    // ********************************
	    // override a private static method
	    // with a public method
	    // ********************************

	    public function setPrivStatBoolean( b:Boolean ) { return true; }
	}
}
