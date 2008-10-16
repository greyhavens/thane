/**
 *  	Class ExtDImplDDOverRidePrivateErr
 *
 *  	This class overrides methods of the parent class
 *  	with private methods in the sub class
 *
 *	Modified: 03/30/2005 (gasingh@macromedia.com) 
 *	Added the package Errors to encapsulate the test case code.
 *	Changed the class name to reflect the file name.
 */

package Errors {

	dynamic class DynamicClass {

	    var boolean:Boolean;
	    public var pubBoolean:Boolean;
		private var privArray:Array;
	    static var statBoolean:Boolean;

	    function getBoolean() : Boolean { return boolean; }
	    public function setPubBoolean() : Boolean { return pubBoolean; }
	    private function setPrivBoolean() : Boolean { return privBoolean; }
	    static function setPrivBoolean() : Boolean { return privBoolean; }

	}

	interface DefaultIntDef {
	     function foo();
	}

	interface DefaultInt {
	     function goo();
	}

	class ExtDImplDDOverRidePrivateErr extends DynamicClass implements DefaultIntDef, DefaultInt {

	    // interface definitions
	    function foo() {}
	    function goo() {}

	    // *******************************
	    // override a default method
	    // with a default method
	    // *******************************

	    private function getBoolean() : Boolean { return true; }

	    // *******************************
	    // override a public method
	    // with a default method
	    // *******************************

	    private function setPubBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a private method
	    // with a default method
	    // *******************************

	    private function setPrivBoolean( b:Boolean ) { return b; }

	    // *******************************
	    // override a static method
	    // with a default method
	    // *******************************

	    private function setStatBoolean( b:Boolean ) { return b; }

	}

	var out = new ExtDImplDDOverRidePrivateErr();

	out.iBaseClass();

	// print( "FAILED, should give compile time error for overriding" );
}
